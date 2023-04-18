cmake_minimum_required(VERSION 3.23)

project(chess)

## If you want to link SFML statically
set(SFML_STATIC_LIBRARIES TRUE)

## Better set in the CMake cache
set(SFML_DIR "lib/GCC 7.3.0 MinGW (SEH) - 64-bit/SFML-2.5.1-windows-gcc-7.3.0-mingw-64-bit/SFML-2.5.1/lib/cmake/SFML")
set(CMAKE_CXX_STANDARD 17)
SET(COMMON_FLAGS "-mcpu=cortex-m3 -O3 -flto -mthumb -ffunction-sections -fdata-sections -g -fno-common -fmessage-length=0")
SET(CMAKE_CXX_FLAGS_INIT "${COMMON_FLAGS} -std=c++17")
set(CMAKE_EXE_LINKER_FLAGS "-static")

find_package(SFML 2.5.1 COMPONENTS graphics REQUIRED)

file(GLOB SRCFILES "src/*.cpp")
add_executable(chess ${SRCFILES})
#add_executable(chess src/main.cpp src/misc.cpp src/magic_bitboard.cpp src/sliding_figure.cpp src/bishop.cpp src/rook.cpp src/queen.cpp src/jumping_figure.cpp src/king.cpp src/knight.cpp src/pawn.cpp src/zobrist_hash.cpp src/repetition_history.cpp src/move_list.cpp src/board.cpp src/material_evaluate.cpp src/positional_evaluate.cpp src/evaluate.cpp src/move_ordering.cpp src/transposition_table.cpp src/search.cpp)
target_link_libraries(chess sfml-graphics -static-libgcc -static-libstdc++)