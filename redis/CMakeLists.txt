cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
project(redis VERSION 1.0)
# project(Tutorial VERSION 1.0)

add_executable(redis redis.cpp)