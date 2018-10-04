cmake_minimum_required(VERSION 2.8.4)
project(ArrayBlockingQueue)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g -Wall -pedantic")
# Generate shared library libArrayBlockingQueue.so file.
ADD_LIBRARY(ArrayBlockingQueue SHARED ArrayBlockingQueue.cpp)

add_executable(Example1.out Example1.cpp)
add_executable(Example2.out Example2.cpp)
target_link_libraries(Example1.out ArrayBlockingQueue)
target_link_libraries(Example2.out ArrayBlockingQueue)

