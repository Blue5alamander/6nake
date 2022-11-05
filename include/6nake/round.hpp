#pragma once


#include <6nake/mapgen.hpp>
#include <6nake/player.hpp>

#include <planet/sdl.hpp>

#include <felspar/coro/stream.hpp>


namespace game {


    struct round {
        round(main &m);

        felspar::coro::task<update::message> play();
        felspar::coro::task<bool> died(update::player);

      private:
        main &game;
        mapgen::hex::world_type world = mapgen::create_map();
        player::snake player = {world};
        planet::affine::point2d looking_at = player.position.centre();
        float scale = calculate_auto_scale_factor() / 1000;
        planet::sdl::renderer renderer = {game.window};
        planet::sdl::panel arena = {renderer};

        std::function<void(void)> hud;

        float calculate_auto_scale_factor() const;
        felspar::coro::stream<planet::sdl::renderer::frame> render();
    };


}
