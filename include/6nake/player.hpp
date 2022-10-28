#pragma once


#include <6nake/mapgen.hpp>
#include <6nake/update.hpp>


namespace player {


    class snake {
        std::size_t turn{};
        std::vector<planet::hexmap::coordinates> occupies;
        long health = 8;
        std::size_t score = {};
        std::size_t vision = {2};
        std::vector<std::size_t> vision_decrease_turn;

      public:
        snake(mapgen::hex::world_type &);

        planet::hexmap::coordinates position;

        update::message
                move(mapgen::hex::world_type &, planet::hexmap::coordinates by);

        auto vision_distance() const noexcept { return vision; }
        auto current_score() const noexcept { return score; }
        auto current_health() const noexcept { return health; }

        std::size_t size() const noexcept { return occupies.size(); }
        planet::hexmap::coordinates operator[](std::size_t s) const {
            return occupies[s];
        }
    };


}
