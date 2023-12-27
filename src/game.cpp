#include <6nake/game.hpp>
#include <6nake/round.hpp>

#include <felspar/coro/eager.hpp>


using namespace std::literals;
using namespace planet::audio::literals;


/**
 * `game::main`
 */


game::main::main(planet::sdl::init &i, std::filesystem::path exe)
: sdl{i},
  assets{std::move(exe)},
  window{sdl, "6nake", SDL_WINDOW_FULLSCREEN_DESKTOP},
  font{assets,
       "Pixellettersfull-BnJ5.ttf",
       window.uzheight() / 10,
       {255, 255, 255, 255}},
  death{assets.file_data("death.ogg")},
  food{assets.file_data("food.ogg")},
  food_plus{assets.file_data("food_plus.ogg")},
  move{assets.file_data("move.ogg")},
  rock{assets.file_data("rock.ogg")},
  vision_plus{assets.file_data("vision_plus.ogg")} {}


felspar::coro::task<int> game::main::run() {
    felspar::coro::eager<> ui;
    ui.post(*this, &main::interface);

    while (true) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT: co_return 0;
            case SDL_MOUSEBUTTONUP:
                switch (event.button.button) {
                case SDL_BUTTON_LEFT:
                    window.baseplate.events.mouse.push(
                            {planet::events::button::left,
                             planet::events::action::up,
                             planet::affine::point2d{
                                     float(event.motion.x),
                                     float(event.motion.y)}});
                    break;
                }
                break;
            }
        }
        if (ui.done()) { co_return 0; }
        co_await sdl.io.sleep(50ms);
    }
}


felspar::coro::task<void> game::main::interface() {
    while (true) {
        game::round round{*this};
        auto outcome = co_await round.play();
        switch (outcome.state) {
        case update::player::alive: co_return;
        case update::player::dead_self: [[fallthrough]];
        case update::player::dead_health:
            if (not co_await round.died(outcome.state)) { co_return; }
            break;
        }
    }
}
