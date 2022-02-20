cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)

project(Factory)

add_executable(simple_factory simple_factory.cpp)
add_executable(factory_method factory_method.cpp)
add_executable(abstract_factory abstract_factory.cpp)