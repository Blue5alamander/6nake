#pragma once


#include <planet/hexmap.hpp>

#include <random>


namespace player {
    class snake;
}


namespace mapgen {


    /// Feature within the hex -- keep `player` last when adding new ones
    enum class feature { none, rock, food, food_plus, vision_plus };


    struct hex {
        feature features = feature::none;
        player::snake *player = nullptr;

        using world_type = planet::hexmap::world_type<hex, 32>;
    };


    hex::world_type create_map();


}
