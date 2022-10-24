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
        ++outcome.length_delta;
        outcome.health_delta -= 2;

        switch (h.features) {
        case mapgen::feature::none: break;
        case mapgen::feature::food:
            outcome.health_delta += 9;
            outcome.score_delta += 5;
            break;
        case mapgen::feature::food_plus:
            outcome.health_delta += 14;
            outcome.score_delta += 6;
            break;
        case mapgen::feature::vision_plus:
            outcome.health_delta += 6;
            outcome.score_delta += 9;
            break;
        case mapgen::feature::rock: outcome.health_delta -= 12; break;
        }
        outcome.consumed = std::exchange(h.features, mapgen::feature::none);
    }

    health += outcome.health_delta;
    score += outcome.score_delta;

    auto const length = health / 8;
    while (occupies.size() > length) {
        occupies.front()->player = nullptr;
        occupies.erase(occupies.begin());
        --outcome.length_delta;
    }

    return outcome;
}
