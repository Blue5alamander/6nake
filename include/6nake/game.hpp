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

        /// If a mouse click is detected it will appear here
        std::optional<planet::affine::point2d> mouse_click;

      private:
        felspar::coro::task<void> interface();
    };


    struct round {
        main &game;
        mapgen::hex::world_type world = mapgen::create_map();
        player::snake player = {world};
        planet::affine::point2d looking_at = player.position.centre();
        float scale = calculate_auto_scale_factor();

        felspar::coro::task<update::message> play();
        felspar::coro::task<void> died(update::player);

      private:
        float calculate_auto_scale_factor() const;
        felspar::coro::stream<planet::affine::point2d> renderer();
    };


}
