#pragma once


#include <6nake/mapgen.hpp>

#include <planet/sdl.hpp>


namespace draw {


    void
            world(planet::sdl::renderer &,
                  planet::ui::panel &,
                  mapgen::hex::world_type const &,
                  player::snake const &,
                  planet::hexmap::coordinates::value_type vision_range);


}
