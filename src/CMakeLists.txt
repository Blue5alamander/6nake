add_executable(6nake
        6nake.cpp
        draw.cpp
        mapgen.cpp
        player.cpp
    )
target_include_directories(6nake PUBLIC ../include/)
target_link_libraries(6nake
        planet-sdl
    )
