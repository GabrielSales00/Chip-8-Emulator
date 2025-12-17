cmake_minimum_required(VERSION 3.10)

project(chip8_emu C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)


find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} include)

file(GLOB SRC_FILES src/*.c)

add_executable(chip8_emu ${SRC_FILES})

target_include_directories(chip8_emu PRIVATE ${SDL2_INCLUDE_DIRS})

target_link_libraries(chip8_emu PRIVATE ${SDL2_LIBRARIES})