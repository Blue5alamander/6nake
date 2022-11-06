#pragma once


#include <planet/sdl.hpp>


namespace game {


    struct main {
        planet::sdl::init &sdl;
        planet::asset_manager assets;
        planet::sdl::window window;
        planet::sdl::font font;
        planet::sdl::panel screen;

        main(planet::sdl::init &, std::filesystem::path);

        felspar::coro::task<int> run();

      private:
        felspar::coro::task<void> interface();
    };


}
