cmake_minimum_required(VERSION 3.16)
project(gisele_sim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(gisele_sim
    src/main.cpp
    src/Game.cpp
  src/Gisele.cpp
  src/Coletavel.cpp
)

target_include_directories(gisele_sim PRIVATE include)

if (MSVC)
  target_compile_options(gisele_sim PRIVATE /W4)
else()
  target_compile_options(gisele_sim PRIVATE -Wall -Wextra -Wpedantic)
endif()
