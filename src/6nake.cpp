#include <6nake/game.hpp>

#include <planet/ostream.hpp>
#include <planet/sdl.hpp>


int main() {
    return planet::sdl::co_main(
                   [](planet::sdl::init &sdl) -> felspar::coro::task<int> {
                       game::main game{sdl};
                       co_return co_await game.run();
                   })
            .get();
}
