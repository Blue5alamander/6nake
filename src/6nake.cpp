#include <6nake/draw.hpp>
#include <6nake/mapgen.hpp>
#include <6nake/player.hpp>

#include <planet/ostream.hpp>
#include <planet/sdl.hpp>

#include <iostream>


int main() {
    planet::sdl::init sdl;
    planet::sdl::window window{sdl, "6nake", 640, 480};
    planet::sdl::renderer renderer{window};

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
        }

        if (towards.mag2() > 2.0f) {
            auto const theta = towards.theta();
            auto const index = std::size_t(6.0f * (theta + 1.0f / 12.0f)) % 6;
            std::cout << "Moving towards " << towards << " theta " << theta
                      << " index " << index << '\n';
            player.move(world, planet::hexmap::directions[index]);
        }

        draw::world(frame, world, player, 6);
    }

    return 0;
}
