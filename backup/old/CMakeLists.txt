cmake_minimum_required(VERSION 3.0.0)
project(hw1 VERSION 0.1.0 LANGUAGES C)
set(MTM_FLAGS_DEBUG "-std=c99 -g --pedantic-errors -Wall -Werror")
set(MTM_FLAGS_RELEASE "${MTM_FLAGS_DEBUG} -DNDEBUG")
set(CMAKE_C_FLAGS "${MTM_FLAGS_DEBUG}")
add_executable(chessSystem_exe chessSystem.c tournament.c game.c player.c map.c chessSystemTestsExample.c)


