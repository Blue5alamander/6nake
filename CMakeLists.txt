cmake_minimum_required(VERSION 3.12)
project(6nake)

add_custom_target(check)

add_subdirectory(libs)
add_subdirectory(src)

add_executable(6nake 6nake.cpp)
target_link_libraries(6nake lib6nake)
