#pragma once


namespace update {


    enum class player { alive, dead_self, dead_health };


    struct message {
        player state = player::alive;
        mapgen::feature consumed = mapgen::feature::none;
    };


}
