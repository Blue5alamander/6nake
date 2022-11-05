#include <6nake/intro.hpp>


game::intro::intro(main &g) : game{g} {}


felspar::coro::task<void> game::intro::show() { co_return; }
