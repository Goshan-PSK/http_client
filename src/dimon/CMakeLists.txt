cmake_minimum_required(VERSION 3.15)

add_library(my string.cpp list.cpp list.hpp map.cpp)

target_compile_features(my PUBLIC cxx_std_23)
