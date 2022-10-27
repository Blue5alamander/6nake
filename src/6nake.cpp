#include <6nake/draw.hpp>
#include <6nake/mapgen.hpp>
#include <6nake/player.hpp>

#include <planet/ostream.hpp>
#include <planet/sdl.hpp>

#include <SDL_ttf.h>

#include <iostream>


int main() {
    planet::sdl::init sdl;
    TTF_Init();
    planet::sdl::window window{sdl, "6nake", 640, 480};
    planet::sdl::renderer renderer{window};

    TTF_Font *font = TTF_OpenFont("Pixellettersfull-BnJ5.ttf", 50);

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

        planet::point2d click_direction{0, 0};

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

        if (click_direction.mag2() > 2.0f) {
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
        auto const target_look_at = player.position.centre();
        constexpr float translate_speed = 0.025f;
        if (target_look_at.x() > looking_at.x()) {
            looking_at.x(std::min(
                    looking_at.x() + translate_speed, target_look_at.x()));
        } else if (target_look_at.x() < looking_at.x()) {
            looking_at.x(std::max(
                    looking_at.x() - translate_speed, target_look_at.x()));
        }
        if (target_look_at.y() > looking_at.y()) {
            looking_at.y(std::min(
                    looking_at.y() + translate_speed, target_look_at.y()));
        } else if (target_look_at.y() < looking_at.y()) {
            looking_at.y(std::max(
                    looking_at.y() - translate_speed, target_look_at.y()));
        }

        draw::world(frame, world, player, player.vision_distance());
        SDL_Color color = {255, 255, 255};
        SDL_Surface *surface = TTF_RenderText_Solid(
                font,
                ("Score: " + std::to_string(player.current_score())).c_str(),
                color);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        int texW = 0;
        int texH = 0;
        SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
        SDL_Rect dstrect = {0, 0, texW, texH};
        SDL_RenderCopy(renderer, texture, nullptr, &dstrect);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
    std::cout << "Your final score was " << player.current_score() << '\n';

    TTF_CloseFont(font);
    TTF_Quit();

    return 0;
}
