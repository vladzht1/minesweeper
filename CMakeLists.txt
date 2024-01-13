cmake_minimum_required(VERSION 3.15)

project(minesweeper)

set(CMAKE_CPP_STANDART 21)
set(CMAKE_DEBUG_POSTFIX d)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")

include_directories(
    minesweeper
    src
    vendor/SFML/include
)

link_directories(
    minesweeper
    vendor/SFML/lib
)

add_executable(
    minesweeper
    ${SOURCES}
)

set_target_properties(minesweeper PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

target_link_libraries(
    minesweeper
    sfml-system
    sfml-graphics
    sfml-window
)
