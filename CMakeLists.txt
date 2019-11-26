cmake_minimum_required(VERSION 3.15)
project(c++i8080-emu)

set(CMAKE_CXX_STANDARD 14)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lmingw32")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")


include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)
include_directories( zip )

#here
link_directories(zip)

add_subdirectory(zip)
add_executable(c++i8080-emu main.cpp i8080.cpp i8080.h Screen.cpp Screen.h)

target_link_libraries(c++i8080-emu SDL2main SDL2 zip)


