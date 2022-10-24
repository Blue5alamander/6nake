#include <6nake/draw.hpp>
#include <6nake/player.hpp>


namespace {
    void draw_hex(
            planet::sdl::drawframe &draw,
            planet::hexmap::coordinates const loc,
            float const radius) {
        auto const vertices = loc.vertices(1.0f, radius);
        std::array<SDL_Point, 7> drawing{};
        for (std::size_t index{}; auto v : vertices) {
            auto const p = draw.viewport.into(v);
            drawing[index++] = {int(p.x()), int(p.y())};
        }
        drawing[6] = drawing[0];
        draw.lines(drawing);
    }
}


void draw::world(
        planet::sdl::drawframe &draw,
        mapgen::hex::world_type const &world,
        player::snake const &player,
        long range) {
    auto const top_left =
            player.position + planet::hexmap::coordinates{-range, range};
    auto const bottom_right = player.position
            + planet::hexmap::coordinates{range + 1, -range - 1};

    for (auto const loc :
         planet::hexmap::coordinates::by_column(top_left, bottom_right)) {
        auto const &cell = world[loc];
        if ((player.position - loc).mag2() <= range * range) {
            if (player.position == loc or cell.player) {
                draw.colour(0, 255, 0);
                draw_hex(draw, loc, 0.8f);
            } else {
                switch (cell.features) {
                case mapgen::feature::none:
                    draw.colour(120, 120, 120);
                    draw_hex(draw, loc, 0.9f);
                    break;
                case mapgen::feature::rock:
                    draw.colour(255, 255, 255);
                    draw_hex(draw, loc, 0.9f);
                    draw_hex(draw, loc, 0.7f);
                    draw_hex(draw, loc, 0.5f);
                    break;
                case mapgen::feature::food:
                    draw.colour(0, 0, 255);
                    draw_hex(draw, loc, 0.3f);
                    break;
                case mapgen::feature::food_plus:
                    draw.colour(120, 120, 255);
                    draw_hex(draw, loc, 0.5f);
                    break;
                case mapgen::feature::vision_plus:
                    draw.colour(255, 120, 255);
                    draw_hex(draw, loc, 0.4f);
                    break;
                }
            }
        }
    }
}
