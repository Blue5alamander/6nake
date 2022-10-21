#include <SDL.h>


int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *const window = SDL_CreateWindow(
            "6nake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
            0);

    for (bool quit = false; not quit;) {
        SDL_Event event;
        SDL_WaitEvent(&event);

        switch (event.type) {
        case SDL_QUIT: quit = true; break;
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
