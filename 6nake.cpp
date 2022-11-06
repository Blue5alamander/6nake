#include <6nake/game.hpp>

#include <planet/ostream.hpp>
#include <planet/sdl.hpp>


int main(int args, char const *argv[]) {
    return planet::sdl::co_main(
            [](planet::sdl::init &sdl, int args,
               char const *argv[]) -> felspar::coro::task<int> {
                game::main game{sdl, {argv[0]}};
                co_return co_await game.run();
            },
            args, argv);
}
