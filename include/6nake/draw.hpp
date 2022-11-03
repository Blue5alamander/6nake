#pragma once


#include <6nake/mapgen.hpp>

#include <planet/sdl.hpp>


namespace draw {


    void
            world(planet::sdl::renderer &,
                  planet::sdl::panel &,
                  mapgen::hex::world_type const &,
                  player::snake const &,
                  long vision_range);


}
