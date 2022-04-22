cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)

project(Composite)

add_executable(safe_composite safe_composite.cpp)
add_executable(transparent_composite transparent_composite.cpp)