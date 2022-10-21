find_package(SDL2 REQUIRED)

add_executable(6nake 6nake.cpp)
include_directories(6nake ${SDL2_INCLUDE_DIRS})
target_link_libraries(6nake
        ${SDL2_LIBRARIES}
    )
