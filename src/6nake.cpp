#include <planet/sdl.hpp>
#include <planet/hexmap.hpp>

#include <iostream>
#include <planet/ostream.hpp>


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

        draw.colour(200, 200, 200);
        auto const vertices =
                planet::hexmap::coordinates{}.vertices(1.0f, 0.9f);
        for (std::size_t index{}; index < vertices.size(); ++index) {
            auto const next = (index + 1) % vertices.size();
            auto const v0 = draw.viewport.into(vertices[index]);
            auto const v1 = draw.viewport.into(vertices[next]);
            draw.line(v0.x(), v0.y(), v1.x(), v1.y());
        }
    }

    return 0;
}
