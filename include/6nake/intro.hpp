#pragma once


#include <6nake/game.hpp>

#include <planet/sdl.hpp>


namespace game {


    class intro {
        main &game;
        planet::sdl::renderer renderer = {game.window};

        felspar::coro::stream<planet::sdl::renderer::frame> render();

      public:
        intro(main &);

        felspar::coro::task<void> show();
    };


}
