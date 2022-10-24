#include <6nake/player.hpp>


player::snake::snake(mapgen::hex::world_type &world) {
    occupies.push_back(&world[position]);
    occupies.back()->player = this;
}


update::message player::snake::move(
        mapgen::hex::world_type &world, planet::hexmap::coordinates const by) {
    position = position + by;
    auto &h = world[position];
    update::message outcome;
    if (h.player) {
        outcome.state = update::player::dead_self;
    } else {
        occupies.push_back(&world[position]);
        occupies.back()->player = this;

        switch (h.features) {
        case mapgen::feature::none: break;
        case mapgen::feature::food: break;
        case mapgen::feature::food_plus: break;
        case mapgen::feature::vision_plus: break;
        case mapgen::feature::rock: break;
        }
    }
    return outcome;
}
