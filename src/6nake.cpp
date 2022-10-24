#include <6nake/draw.hpp>
#include <6nake/mapgen.hpp>
#include <6nake/player.hpp>

#include <planet/sdl.hpp>


int main() {
    planet::sdl::init sdl;
    planet::sdl::window window{sdl, "6nake", 640, 480};
    planet::sdl::renderer renderer{window};

    auto world = mapgen::create_map();
    player::snake player{world};


    for (bool quit = false; not quit;) {
        SDL_Delay(10);

        SDL_Event event;
        SDL_WaitEvent(&event);

        switch (event.type) {
        case SDL_QUIT: quit = true; break;
        }

        auto frame = renderer(5, 5, 5);
        draw::world(frame, world, player, 2);
    }

    return 0;
}
