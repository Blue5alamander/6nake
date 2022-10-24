#include <6nake/draw.hpp>
#include <6nake/player.hpp>


void draw::world(
        planet::sdl::drawframe &draw,
        mapgen::hex::world_type const &world,
        player::snake const &player,
        long range) {
    auto const top_left =
            player.position + planet::hexmap::coordinates{-range, range};
    auto const bottom_right = player.position
            + planet::hexmap::coordinates{range + 1, -range - 1};

    draw.colour(200, 200, 200);
    for (auto const loc :
         planet::hexmap::coordinates::by_column(top_left, bottom_right)) {
        auto const vertices = loc.vertices(1.0f, 0.9f);
        for (std::size_t index{}; index < vertices.size(); ++index) {
            auto const next = (index + 1) % vertices.size();
            auto const v0 = draw.viewport.into(vertices[index]);
            auto const v1 = draw.viewport.into(vertices[next]);
            draw.line(v0.x(), v0.y(), v1.x(), v1.y());
        }
    }
}
