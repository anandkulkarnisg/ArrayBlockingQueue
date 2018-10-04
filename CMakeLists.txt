cmake_minimum_required(VERSION 2.8.4)
project(ArrayBlockingQueue)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g -Wall -pedantic")
# Generate shared library libArrayBlockingQueue.so file.
ADD_LIBRARY(ArrayBlockingQueue SHARED ArrayBlockingQueue.cpp)

