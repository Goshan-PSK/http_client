cmake_minimum_required(VERSION 3.22)
project(my LANGUAGES CXX)

include_directories("src")

add_subdirectory("src/dimon")

find_package(Catch2 3 REQUIRED)

add_executable(test test.cpp)
target_link_libraries(test my)

add_executable(animal test_animal.cpp)
target_link_libraries(animal my)

add_executable(list test_list.cpp)
target_link_libraries(list my)

add_executable(test_map test_map.cpp)
target_link_libraries(
    test_map
    PUBLIC my
    PRIVATE Catch2::Catch2WithMain)


