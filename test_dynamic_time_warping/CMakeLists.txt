cmake_minimum_required(VERSION 3.10)
project(shift_points)

set(CMAKE_CXX_FLAGS "-std=c++14 ${CMAKE_CXX_FLAGS} -O3")
find_package(fmt)
find_package(Eigen3 REQUIRED)
include_directories(include)

add_executable(shift_points main.cpp)
target_link_libraries(shift_points PRIVATE fmt::fmt Eigen3::Eigen)
