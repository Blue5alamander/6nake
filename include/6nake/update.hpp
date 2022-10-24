#pragma once


namespace update {


    enum class player { alive, dead_self, dead_health };


    struct message {
        player state = player::alive;
        mapgen::feature consumed = mapgen::feature::none;
        long length_delta = {};
        long health_delta = {};
        std::size_t score_delta = {};
        std::size_t vision_distance_delta = {};
    };


}
