cmake_minimum_required(VERSION 3.12)
project(6nake)

add_custom_target(check)

add_subdirectory(libs)

add_executable(6nake
        src/6nake.cpp
        src/draw.cpp
        src/mapgen.cpp
        src/player.cpp
    )
target_include_directories(6nake PUBLIC ./include/)
target_link_libraries(6nake
        planet-sdl
    )
