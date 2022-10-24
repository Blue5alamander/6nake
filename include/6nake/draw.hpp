#pragma once


#include <6nake/mapgen.hpp>

#include <planet/sdl.hpp>


namespace draw {


    void
            world(planet::sdl::drawframe &draw,
                  mapgen::hex::world_type const &world,
                  player::snake const &player,
                  long range);


}
