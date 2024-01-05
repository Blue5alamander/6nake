#include <6nake/draw.hpp>
#include <6nake/game.hpp>
#include <6nake/round.hpp>

#include <planet/ui.hpp>


using namespace std::literals;


/// ## `game::round`


game::round::round(main &g) : game{g} {
    game.screen.add_child(
            arena, {0, 0},
            {float(game.window.width()), float(game.window.height())});
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
        renderer.copy(health, game.window.width() - health_size.width, 0);
    };
    auto mouse = game.window.baseplate.events.mouse.values();
    while (true) {
        auto click = co_await mouse.next();
        auto const move =
                arena.outof(click.location) - player.position.centre();
        if (move.mag2() > 1.0f) {
            auto const theta = move.theta();
            auto const index = std::size_t(6.0f * (theta + 1.0f / 12.0f)) % 6;
            auto const outcome =
                    player.move(world, planet::hexmap::directions[index]);
            switch (outcome.consumed) {
            case mapgen::feature::none:
                game.sound.trigger(game.move.stereo());
                break;
            case mapgen::feature::rock:
                game.sound.trigger(game.rock.stereo());
                break;
            case mapgen::feature::food:
                game.sound.trigger(game.food.stereo());
                break;
            case mapgen::feature::food_plus:
                game.sound.trigger(game.food_plus.stereo());
                break;
            case mapgen::feature::vision_plus:
                game.sound.trigger(game.vision_plus.stereo());
                break;
            }

            if (outcome.state != update::player::alive) { co_return outcome; }
        }
    }
}


felspar::coro::task<bool> game::round::died(update::player reason) {
    game.sound.trigger(game.death.stereo());

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

    planet::queue::pmc<bool> choice;
    planet::ui::button again{
            planet::sdl::texture{renderer, game.font.render("Play again")},
            choice, true},
            quit{planet::sdl::texture{renderer, game.font.render("Quit")},
                 choice, false};

    hud = [&, this]() {
        auto const score_size = score.extents();
        renderer.copy(
                score, (game.window.width() - score_size.width) / 2,
                game.window.height() / 2);

        auto const text_size = text.extents();
        renderer.copy(
                text, (game.window.width() - text_size.width) / 2,
                game.window.height() / 3);

        again.draw();
        quit.draw();
    };

    co_await game.sdl.io.sleep(2s);
    again.add_to(game.window.baseplate, game.screen);
    quit.add_to(game.window.baseplate, game.screen);
    bool const go_again = co_await choice.values().next();
    co_await game.sdl.io.sleep(10ms);
    co_return go_again;
}


float game::round::calculate_auto_scale_factor() const {
    return std::min(game.window.width(), game.window.height())
            / (1.0f + 2.0f * player.vision_distance());
}


felspar::coro::stream<planet::sdl::renderer::frame> game::round::render() {
    /// Auto scaling with scaling animation
    while (true) {
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

        co_yield renderer.present();
        co_await game.sdl.io.sleep(10ms);
    }
}
