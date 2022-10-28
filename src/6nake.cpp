#include <6nake/draw.hpp>
#include <6nake/mapgen.hpp>
#include <6nake/player.hpp>

#include <planet/ostream.hpp>
#include <planet/sdl.hpp>

#include <iostream>


int main() {
    planet::sdl::init sdl;
    planet::sdl::ttf text{sdl};
    planet::sdl::window window{sdl, "6nake", 640, 480};
    planet::sdl::font font{"Pixellettersfull-BnJ5.ttf", window.height() / 10};
    planet::sdl::renderer renderer{window};

    auto world = mapgen::create_map();
    player::snake player{world};

    /// Auto scaling with scaling animation
    auto const auto_scale = [&window, &player]() {
        return std::min(window.width(), window.height())
                / (1.0f + 2.0f * player.vision_distance());
    };
    auto scale = auto_scale();

    auto looking_at = player.position.centre();

    for (bool quit = false; not quit;) {
        SDL_Delay(10);

        auto frame = renderer(5, 5, 5);
        frame.viewport.translate(-looking_at)
                .reflect_y()
                .scale(scale)
                .translate({window.width() / 2.0f, window.height() / 2.0f});

        planet::affine::point2d click_direction{0, 0};

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT: quit = true; break;
            case SDL_MOUSEBUTTONUP:
                switch (event.button.button) {
                case SDL_BUTTON_LEFT:
                    click_direction = frame.viewport.outof(
                                              {float(event.motion.x),
                                               float(event.motion.y)})
                            - player.position.centre();
                    break;
                }
                break;
            }
        }

        if (click_direction.mag2() > 1.0f) {
            auto const theta = click_direction.theta();
            auto const index = std::size_t(6.0f * (theta + 1.0f / 12.0f)) % 6;
            auto const outcome =
                    player.move(world, planet::hexmap::directions[index]);

            if (outcome.state != update::player::alive) {
                std::cout << "You died. ";
                quit = true;
            }
        }

        /// Next view location
        auto target_scale = auto_scale();
        auto scale_movement = 3.5f / target_scale;
        if (target_scale > scale) {
            scale = std::min(scale + scale_movement, target_scale);
        } else if (target_scale < scale) {
            scale = std::max(scale - scale_movement, target_scale);
        }

        constexpr float translate_speed = 0.025f;
        constexpr float translate_speed2 = translate_speed * translate_speed;
        auto const target_look_at = player.position.centre();
        auto const direction = target_look_at - looking_at;
        if (direction.mag2() <= translate_speed2) {
            looking_at = target_look_at;
        } else {
            auto const translate = planet::affine::point2d::from_polar(
                    translate_speed, direction.theta());
            looking_at = looking_at + translate;
        }

        draw::world(frame, world, player, player.vision_distance());
        planet::sdl::texture score{
                renderer,
                font.render(
                        ("Score: " + std::to_string(player.current_score()))
                                .c_str(),
                        {255, 255, 255})};
        auto const score_size = score.extents();
        SDL_RenderCopy(renderer.get(), score.get(), nullptr, &score_size);
        planet::sdl::texture health{
                renderer,
                font.render(
                        ("Health: " + std::to_string(player.current_health()))
                                .c_str(),
                        {255, 255, 255})};
        auto const health_size = health.extents();
        SDL_Rect health_pos = {
                int(window.width()) - health_size.w, 0, health_size.w,
                health_size.h};
        SDL_RenderCopy(renderer.get(), health.get(), nullptr, &health_pos);
    }
    std::cout << "Your final score was " << player.current_score() << '\n';

    return 0;
}
