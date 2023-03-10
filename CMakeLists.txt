cmake_minimum_required(VERSION 3.1)

project(chess)

## If you want to link SFML statically
set(SFML_STATIC_LIBRARIES TRUE)

## Better set in the CMake cache
set(SFML_DIR "lib/GCC 7.3.0 MinGW (SEH) - 64-bit/SFML-2.5.1-windows-gcc-7.3.0-mingw-64-bit/SFML-2.5.1/lib/cmake/SFML")

SET(COMMON_FLAGS "-mcpu=cortex-m3 -O3 -flto -mthumb -ffunction-sections -fdata-sections -g -fno-common -fmessage-length=0")
SET(CMAKE_CXX_FLAGS_INIT "${COMMON_FLAGS} -std=c++17")
set(CMAKE_EXE_LINKER_FLAGS "-static")

find_package(SFML 2.5.1 COMPONENTS graphics REQUIRED)
add_executable(chess src/main.cpp)
target_link_libraries(chess sfml-graphics -static-libgcc -static-libstdc++)
