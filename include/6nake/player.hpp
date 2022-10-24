#pragma once


#include <6nake/mapgen.hpp>
#include <6nake/update.hpp>


namespace player {


    class snake {
        std::vector<mapgen::hex *> occupies;

      public:
        snake(mapgen::hex::world_type &);

        planet::hexmap::coordinates position;

        update::message
                move(mapgen::hex::world_type &, planet::hexmap::coordinates by);
    };


}