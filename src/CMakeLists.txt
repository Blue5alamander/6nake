add_library(lib6nake
        draw.cpp
        game.cpp
        mapgen.cpp
        player.cpp
        round.cpp
    )
target_include_directories(lib6nake PUBLIC ../include/)
target_link_libraries(lib6nake
        planet-sdl
    )
