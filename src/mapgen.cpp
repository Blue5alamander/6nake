#include <6nake/mapgen.hpp>


namespace {


    using prng_type = std::mt19937;
    using distribution_type = std::uniform_real_distribution<float>;


    /// The probability increases with distance
    inline auto increasing(float control) {
        return [control](
                       auto &generator, auto &distribution,
                       float const distance) -> bool {
            auto const probability = distance / (distance + control);
            return distribution(generator) < probability;
        };
    }
    inline auto decreasing(float control) {
        return [control](
                       auto &generator, auto &distribution,
                       float const distance) -> bool {
            auto const probability = distance / (distance + control);
            return distribution(generator) > probability;
        };
    }


    /// Determine how a feature is generated.
    struct generate_feature {
        mapgen::feature creates;
        std::function<auto(std::mt19937 &,
                           std::uniform_real_distribution<float> &,
                           float)
                              ->bool>
                determine;
    };


    std::array<generate_feature, 5> const map_options = {
            generate_feature{
                    mapgen::feature::none,
                    [](auto &, auto &, float const distance) {
                        return distance <= 1.0f;
                    }},
            generate_feature{mapgen::feature::rock, increasing(16.0f)},
            generate_feature{mapgen::feature::food, decreasing(0.6f)},
            generate_feature{mapgen::feature::food_plus, increasing(100.0f)},
            generate_feature{mapgen::feature::vision_plus, increasing(200.0f)}};


}


mapgen::hex::world_type mapgen::create_map() {
    std::random_device rd;
    return {{},
            [gen = std::mt19937(rd()),
             distribution = std::uniform_real_distribution<float>(0.0f, 1.0f)](
                    auto const p) mutable {
                auto const dist = std::sqrt(p.mag2());
                for (auto const &f : map_options) {
                    if (f.determine(gen, distribution, dist)) {
                        return hex{f.creates};
                    }
                }
                return hex{};
            }};
}
