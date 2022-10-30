#include <6nake/draw.hpp>
#include <6nake/game.hpp>

#include <felspar/coro/start.hpp>


using namespace std::literals;


/**
 * `game::main`
 */


game::main::main(planet::sdl::init &i)
: sdl{i},
  window{sdl, "6nake", SDL_WINDOW_FULLSCREEN_DESKTOP},
  font{"Pixellettersfull-BnJ5.ttf", window.height() / 10} {}


felspar::coro::task<int> game::main::run() {
    felspar::coro::starter ui;
    ui.post(*this, &main::interface);

    while (true) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT: co_return 0;
            case SDL_MOUSEBUTTONUP:
                switch (event.button.button) {
                case SDL_BUTTON_LEFT:
                    mouse_click = planet::affine::point2d{
                            float(event.motion.x), float(event.motion.y)};
                    break;
                }
                break;
            }
        }
        ui.garbage_collect_completed();
        if (ui.empty()) { co_return 0; }
        co_await sdl.io.sleep(50ms);
    }
}


felspar::coro::task<void> game::main::interface() {
    while (true) {
        game::round round{*this};
        auto outcome = co_await round.play();
        switch (outcome.state) {
        case update::player::alive: co_return;
        case update::player::dead_self: [[fallthrough]];
        case update::player::dead_health:
            co_await round.died(outcome.state);
            break;
        }
    }
}


/**
 * `game::round`
 */


game::round::round(main &g) : game{g} {
    renderer.connect(*this, &round::render);
}


felspar::coro::task<update::message> game::round::play() {
    for (auto moves = clicks(); auto move = co_await moves.next();) {
        if (move->mag2() > 1.0f) {
            auto const theta = move->theta();
            auto const index = std::size_t(6.0f * (theta + 1.0f / 12.0f)) % 6;
            auto const outcome =
                    player.move(world, planet::hexmap::directions[index]);

            if (outcome.state != update::player::alive) { co_return outcome; }
        }
    }
    co_return {};
}


felspar::coro::task<void> game::round::died(update::player reason) {
    char const *explanation = nullptr;
    switch (reason) {
    case update::player::alive: co_return; // This shouldn't have happened
    case update::player::dead_self:
        explanation = "You can't eat yourself, so you died!";
        break;
    case update::player::dead_health:
        explanation = "You ran out of health and died from exhaustion";
        break;
    }
    auto const text = game.font.render(explanation, {255, 255, 255});

    for (bool quit = false; not quit;) {
        renderer.colour(5, 5, 5);
        renderer.clear();

        arena.viewport = {};
        arena.viewport.translate(-looking_at)
                .reflect_y()
                .scale(scale)
                .translate(
                        {game.window.width() / 2.0f,
                         game.window.height() / 2.0f});

        draw::world(renderer, arena, world, player, player.vision_distance());
        planet::sdl::texture texture{renderer, text};
        auto const text_size = texture.extents();
        renderer.copy(
                texture, (game.window.width() - text_size.w) / 2,
                game.window.height() / 3);

        renderer.present();
        co_await game.sdl.io.sleep(10ms);
    }
    co_return;
}


float game::round::calculate_auto_scale_factor() const {
    return std::min(game.window.width(), game.window.height())
            / (1.0f + 2.0f * player.vision_distance());
}


felspar::coro::stream<planet::affine::point2d> game::round::clicks() {
    while (true) {
        auto click = std::exchange(game.mouse_click, {});
        if (click) {
            co_yield arena.viewport.outof(*click) - player.position.centre();
        }
        co_await renderer.next_frame();
    }
}


felspar::coro::stream<planet::sdl::renderer::frame> game::round::render() {
    std::size_t number{};
    /// Auto scaling with scaling animation
    for (bool quit = false; not quit;) {
        ++number;

        renderer.colour(5, 5, 5);
        renderer.clear();

        arena.viewport = {};
        arena.viewport.translate(-looking_at)
                .reflect_y()
                .scale(scale)
                .translate(
                        {game.window.width() / 2.0f,
                         game.window.height() / 2.0f});

        auto const scale_difference = calculate_auto_scale_factor() - scale;
        scale += scale_difference * 0.15f;

        auto const target_look_at = player.position.centre();
        auto const direction = target_look_at - looking_at;
        if (direction.mag2() <= 0.0001f) {
            looking_at = target_look_at;
        } else {
            auto const translate = planet::affine::point2d::from_polar(
                    std::sqrt(direction.mag2()) * 0.1f, direction.theta());
            looking_at = looking_at + translate;
        }

        draw::world(renderer, arena, world, player, player.vision_distance());
        planet::sdl::texture score{
                renderer,
                game.font.render(
                        ("Score: " + std::to_string(player.current_score()))
                                .c_str(),
                        {255, 255, 255})};
        renderer.copy(score, 0, 0);
        planet::sdl::texture health{
                renderer,
                game.font.render(
                        ("Health: " + std::to_string(player.current_health()))
                                .c_str(),
                        {255, 255, 255})};
        auto const health_size = health.extents();
        renderer.copy(health, game.window.width() - health_size.w, 0);

        renderer.present();
        co_yield planet::sdl::renderer::frame{number};
        co_await game.sdl.io.sleep(10ms);
    }
}
