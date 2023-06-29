#include <6nake/draw.hpp>
#include <6nake/player.hpp>


void draw::world(
        planet::sdl::renderer &renderer,
        planet::ui::panel &draw,
        mapgen::hex::world_type const &world,
        player::snake const &player,
        planet::hexmap::coordinates::value_type const range) {
    auto const top_left =
            player.position + planet::hexmap::coordinates{-range, range};
    auto const bottom_right = player.position
            + planet::hexmap::coordinates{range + 1, -range - 1};

    for (auto const loc :
         planet::hexmap::coordinates::by_column(top_left, bottom_right)) {
        auto const &cell = world[loc];
        if ((player.position - loc).mag2() <= range * range) {
            if (player.position == loc) {
                renderer.colour(0, 255, 0);
                draw_hex(renderer, draw, loc, 0.8f);
            } else if (not cell.player) {
                switch (cell.features) {
                case mapgen::feature::none:
                    renderer.colour(120, 120, 120);
                    draw_hex(renderer, draw, loc, 0.9f);
                    break;
                case mapgen::feature::rock:
                    renderer.colour(255, 255, 255);
                    draw_hex(renderer, draw, loc, 0.9f);
                    draw_hex(renderer, draw, loc, 0.7f);
                    draw_hex(renderer, draw, loc, 0.5f);
                    break;
                case mapgen::feature::food:
                    renderer.colour(0, 0, 255);
                    draw_hex(renderer, draw, loc, 0.3f);
                    break;
                case mapgen::feature::food_plus:
                    renderer.colour(120, 120, 255);
                    draw_hex(renderer, draw, loc, 0.5f);
                    break;
                case mapgen::feature::vision_plus:
                    renderer.colour(255, 120, 255);
                    draw_hex(renderer, draw, loc, 0.4f);
                    break;
                }
            }
        }
        if (player.size()) {
            renderer.colour(0, 255, 0);
            for (std::size_t index{}; index < player.size() - 1; ++index) {
                auto const p = player[index];
                draw_hex(renderer, draw, p, 0.6f);
                draw.line(renderer, p.centre(), player[index + 1].centre());
            }
        }
    }
}
