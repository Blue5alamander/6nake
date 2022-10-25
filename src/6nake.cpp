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

    for (bool quit = false; not quit;) {
        SDL_Event event;
        SDL_WaitEvent(&event);

        auto const looking_at = player.position.centre();
        auto frame = renderer(5, 5, 5, looking_at);

        planet::point2d towards{0, 0};
        switch (event.type) {
        case SDL_QUIT: quit = true; break;
        case SDL_MOUSEBUTTONUP:
            switch (event.button.button) {
            case SDL_BUTTON_LEFT:
                towards =
                        frame.viewport.outof(
                                {float(event.motion.x), float(event.motion.y)})
                        - looking_at;
                break;
            }
            break;
        }

        if (towards.mag2() > 2.0f) {
            auto const theta = towards.theta();
            auto const index = std::size_t(6.0f * (theta + 1.0f / 12.0f)) % 6;
            auto const outcome =
                    player.move(world, planet::hexmap::directions[index]);

            if (outcome.state != update::player::alive) {
                std::cout << "You died. Your final score was "
                          << player.current_score() << '\n';
                quit = true;
            }
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

    TTF_CloseFont(font);
    TTF_Quit();
    return 0;
}
