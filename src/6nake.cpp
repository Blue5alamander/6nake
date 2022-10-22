#include <planet/sdl.hpp>


int main() {
    planet::sdl::init sdl;
    planet::sdl::window window{sdl, "6nake", 640, 480};
    planet::sdl::renderer renderer{window};

    for (bool quit = false; not quit;) {
        SDL_Delay(10);

        SDL_Event event;
        SDL_WaitEvent(&event);

        switch (event.type) {
        case SDL_QUIT: quit = true; break;
        }

        auto draw = renderer(5, 5, 5);
    }

    return 0;
}
