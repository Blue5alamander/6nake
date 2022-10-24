#pragma once


#include <6nake/mapgen.hpp>


namespace player {


    class snake {
      public:
        snake(mapgen::hex::world_type &);

        planet::hexmap::coordinates position;
    };


}
