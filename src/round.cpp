#include <6nake/draw.hpp>
#include <6nake/game.hpp>


using namespace std::literals;


/**
 * `game::round`
 */


game::round::round(main &g) : game{g} {
    game.screen.add_child(
            arena, {0, 0},
            {float(game.window.width()), float(game.window.height())});
    renderer.connect(*this, &round::render);
}


felspar::coro::task<update::message> game::round::play() {
    hud = [this]() {
        planet::sdl::texture score{
                renderer,
                game.font.render(
                        ("Score: " + std::to_string(player.current_score()))
                                .c_str())};
        renderer.copy(score, 0, 0);
        planet::sdl::texture health{
                renderer,
                game.font.render(
                        ("Health: " + std::to_string(player.current_health()))
                                .c_str())};
        auto const health_size = health.extents();
        renderer.copy(health, game.window.width() - health_size.w, 0);
    };
    while (true) {
        auto click = co_await game.screen.mouse_click.next();
        auto const move = arena.outof(click) - player.position.centre();
        if (move.mag2() > 1.0f) {
            auto const theta = move.theta();
            auto const index = std::size_t(6.0f * (theta + 1.0f / 12.0f)) % 6;
            auto const outcome =
                    player.move(world, planet::hexmap::directions[index]);

            if (outcome.state != update::player::alive) { co_return outcome; }
        }
    }
}


template<typename R>
class text_button {
  public:
    planet::sdl::renderer &renderer;
    planet::sdl::panel panel;
    planet::sdl::texture graphic;
    planet::affine::point2d top_left = {0, 0}, bottom_right = {0, 0};
    bool visible = false;

    R press_value;
    felspar::coro::bus<R> &output_to;
    felspar::coro::eager<> response;

    text_button(
            planet::sdl::renderer &r,
            planet::sdl::surface text,
            felspar::coro::bus<R> &o,
            R v)
    : renderer{r},
      panel{r},
      graphic{r, std::move(text)},
      press_value{v},
      output_to{o} {
        auto const sz = graphic.extents();
        top_left = {sz.w / -2.0f, sz.h / -2.0f};
        bottom_right = -top_left;
    }

    void
            add_to(planet::sdl::panel &parent,
                   planet::affine::point2d const centre) {
        parent.add_child(panel, top_left + centre, bottom_right + centre);
        response.post(*this, &text_button::button_response);
        visible = true;
    }
    void draw() const {
        if (visible) { panel.copy(graphic, {0, 0}); }
    }

    felspar::coro::task<void> button_response() {
        while (true) {
            co_await panel.mouse_click.next();
            output_to.push(press_value);
        }
    }
};


felspar::coro::task<bool> game::round::died(update::player reason) {
    char const *explanation = nullptr;
    switch (reason) {
    case update::player::alive: co_return false; // This shouldn't have happened
    case update::player::dead_self:
        explanation = "You can't eat yourself, so you died!";
        break;
    case update::player::dead_health:
        explanation = "You ran out of health and died from exhaustion";
        break;
    }
    auto const text =
            planet::sdl::texture{renderer, game.font.render(explanation)};

    auto const score = planet::sdl::texture{
            renderer,
            game.font.render(("Your final score is "
                              + std::to_string(player.current_score()))
                                     .c_str())};

    felspar::coro::bus<bool> choice;
    text_button<bool> again{
            renderer, game.font.render("Play again"), choice, true},
            quit{renderer, game.font.render("Quit"), choice, false};

    hud = [&, this]() {
        auto const score_size = score.extents();
        renderer.copy(
                score, (game.window.width() - score_size.w) / 2,
                game.window.height() / 2);

        auto const text_size = text.extents();
        renderer.copy(
                text, (game.window.width() - text_size.w) / 2,
                game.window.height() / 3);

        again.draw();
        quit.draw();
    };

    co_await game.sdl.io.sleep(2s);
    again.add_to(
            game.screen,
            {game.window.width() / 3.0f, 2.0f * game.window.height() / 3.0f});
    quit.add_to(
            game.screen,
            {2.0f * game.window.width() / 3.0f,
             2.0f * game.window.height() / 3.0f});
    bool const go_again = co_await choice.next();
    co_await game.sdl.io.sleep(10ms);
    co_return go_again;
}


float game::round::calculate_auto_scale_factor() const {
    return std::min(game.window.width(), game.window.height())
            / (1.0f + 2.0f * player.vision_distance());
}


felspar::coro::stream<planet::sdl::renderer::frame> game::round::render() {
    std::size_t number{};
    /// Auto scaling with scaling animation
    for (bool quit = false; not quit;) {
        ++number;

        renderer.colour(5, 5, 5);
        renderer.clear();

        arena.reset_coordinate_space();
        arena.translate(-looking_at)
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
        if (hud) { hud(); }

        renderer.present();
        co_yield planet::sdl::renderer::frame{number};
        co_await game.sdl.io.sleep(10ms);
    }
}
