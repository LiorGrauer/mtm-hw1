cmake_minimum_required(VERSION 3.0.0)
project(hw1 VERSION 0.1.0 LANGUAGES C)
set(MTM_FLAGS_DEBUG "-std=c99 --pedantic-errors -Wall -Werror")
set(MTM_FLAGS_RELEASE "${MTM_FLAGS_DEBUG} -DNDEBUG")
set(CMAKE_C_FLAGS "${MTM_FLAGS_DEBUG}")

add_executable(chess chessSystem.c tournament.c ./mtm_map/map.c ./mtm_map/map_node.c game.c player.c ./tests/chessSystemTestsExample_nonServer2.c)



