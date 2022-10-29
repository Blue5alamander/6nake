#pragma once


#include <6nake/mapgen.hpp>
#include <6nake/player.hpp>

#include <planet/sdl.hpp>

#include <felspar/coro/stream.hpp>


namespace game {


    struct main {
        planet::sdl::init &sdl;
        planet::sdl::window window;
        planet::sdl::font font;
        planet::sdl::renderer renderer;

        main(planet::sdl::init &);

        felspar::coro::task<int> run();
    };


    struct round {
        main &game;
        mapgen::hex::world_type world = mapgen::create_map();
        player::snake player = {world};

        felspar::coro::task<update::message> play();

      private:
        felspar::coro::stream<planet::affine::point2d> renderer();
    };


}
