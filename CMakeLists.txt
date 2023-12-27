cmake_minimum_required(VERSION 3.12)
project(6nake)

add_custom_target(check)

add_compile_options(
        -Werror
        -fdiagnostics-color=always
        -fdiagnostics-show-option
        -Wall -Wextra -Wpedantic
    )

add_subdirectory(lib)
add_subdirectory(src)

add_executable(6nake 6nake.cpp)
target_link_libraries(6nake lib6nake)
install(TARGETS 6nake EXPORT 6nake RUNTIME DESTINATION bin)
install(DIRECTORY var/ DESTINATION share/)
