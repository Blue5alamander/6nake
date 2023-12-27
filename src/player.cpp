#include <6nake/player.hpp>


player::snake::snake(mapgen::hex::world_type &world) {
    occupies.push_back(position);
    world[position].player = this;
}


update::message player::snake::move(
        mapgen::hex::world_type &world, planet::hexmap::coordinates const by) {
    ++turn;
    position = position + by;
    auto &h = world[position];
    update::message outcome{};

    occupies.push_back(position);
    ++outcome.length_delta;
    outcome.health_delta -= 1;

    if (h.player) {
        outcome.state = update::player::dead_self;
    } else {
        h.player = this;
        switch (h.features) {
        case mapgen::feature::none: break;
        case mapgen::feature::food:
            outcome.health_delta += 9;
            outcome.score_delta += 5;
            break;
        case mapgen::feature::food_plus:
            outcome.health_delta += 16;
            outcome.score_delta += 6;
            break;
        case mapgen::feature::vision_plus:
            outcome.health_delta += 4;
            outcome.score_delta += 9;
            outcome.vision_distance_delta += 2;
            vision_decrease_turn.push_back(turn + 8);
            break;
        case mapgen::feature::rock: outcome.health_delta -= 12; break;
        }
        outcome.consumed = std::exchange(h.features, mapgen::feature::none);
    }

    if (not vision_decrease_turn.empty()
        and vision_decrease_turn.front() == turn) {
        outcome.vision_distance_delta -= 2;
        vision_decrease_turn.erase(vision_decrease_turn.begin());
    }

    health += outcome.health_delta;
    score += outcome.score_delta;
    vision += outcome.vision_distance_delta;
    if (health <= 0 and outcome.state == update::player::alive) {
        outcome.state = update::player::dead_health;
    } else if (outcome.state == update::player::alive) {
        auto const length = health / 8;
        while (length > 0
               and occupies.size() > static_cast<std::size_t>(length)) {
            world[occupies.front()].player = nullptr;
            occupies.erase(occupies.begin());
            --outcome.length_delta;
        }
    }

    return outcome;
}
