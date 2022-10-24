#pragma once


#include <6nake/mapgen.hpp>
#include <6nake/update.hpp>


namespace player {


    class snake {
        std::vector<mapgen::hex *> occupies;
        long health = 8;
        std::size_t score = {};
        std::size_t vision = {2};

      public:
        snake(mapgen::hex::world_type &);

        planet::hexmap::coordinates position;

        update::message
                move(mapgen::hex::world_type &, planet::hexmap::coordinates by);

        auto vision_distance() const noexcept { return vision; }
    };


}
