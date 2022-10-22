#include <planet/sdl.hpp>


int main() {
    planet::sdl::init sdl;

    SDL_Window *const window = SDL_CreateWindow(
            "6nake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
            0);
    SDL_Renderer *const renderer = SDL_CreateRenderer(window, -1, 0);


    for (bool quit = false; not quit;) {
        SDL_Delay(10);

        SDL_Event event;
        SDL_WaitEvent(&event);

        switch (event.type) {
        case SDL_QUIT: quit = true; break;
        }

        SDL_SetRenderDrawColor(renderer, 5, 5, 5, 255);
        SDL_RenderClear(renderer);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return 0;
}
