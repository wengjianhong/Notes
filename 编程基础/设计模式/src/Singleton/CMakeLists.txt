cmake_minimum_required(VERSION 3.10)
project(Singleton)

# set(CMAKE_CXX_STANDARD 11)

add_executable(singleton_safe singleton_safe.cpp)
add_executable(singleton_unsafe singleton_unsafe.cpp)

find_package(Threads)
target_link_libraries(singleton_safe ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(singleton_unsafe ${CMAKE_THREAD_LIBS_INIT})
