#pragma once


#include <6nake/mapgen.hpp>
#include <6nake/player.hpp>

#include <planet/sdl.hpp>

#include <felspar/coro/start.hpp>
#include <felspar/coro/stream.hpp>


namespace game {


    struct main {
        planet::sdl::init &sdl;
        planet::sdl::window window;
        planet::sdl::font font;

        main(planet::sdl::init &);

        felspar::coro::task<int> run();

        /// If a mouse click is detected it will appear here
        felspar::coro::bus<planet::affine::point2d> mouse_click;

      private:
        felspar::coro::task<void> interface();
    };


    struct round {
        round(main &m);

        felspar::coro::task<update::message> play();
        felspar::coro::task<void> died(update::player);

      private:
        main &game;
        mapgen::hex::world_type world = mapgen::create_map();
        player::snake player = {world};
        planet::affine::point2d looking_at = player.position.centre();
        float scale = calculate_auto_scale_factor();
        planet::sdl::renderer renderer = {game.window};
        planet::sdl::drawframe arena = {renderer};

        std::function<void(void)> hud;

        float calculate_auto_scale_factor() const;
        felspar::coro::stream<planet::sdl::renderer::frame> render();
    };


}
