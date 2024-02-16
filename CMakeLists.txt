cmake_minimum_required(VERSION 3.22)
project(my LANGUAGES CXX)

add_subdirectory("src/dimon")

find_package(Catch2 3 REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)
find_package(OpenSSL REQUIRED)

include_directories("src" ${Boost_INCLUDE_DIR} ${OPENSSL_INCLUDE_DIR})

target_link_libraries(my OpenSSL::SSL)

add_executable(test test.cpp)
target_link_libraries(test my)

add_executable(animal test_animal.cpp)
target_link_libraries(animal my)

add_executable(list test_list.cpp)
target_link_libraries(list my)

add_executable(bit_vector test_bit_vector.cpp)
target_link_libraries(
    bit_vector
    PUBLIC my
    PRIVATE Catch2::Catch2WithMain)

add_executable(test_map test_map.cpp)
target_link_libraries(
    test_map
    PUBLIC my
    PRIVATE Catch2::Catch2WithMain)

    add_executable(test_http test_http.cpp)
target_link_libraries(
    test_http
    PUBLIC my
    PRIVATE Catch2::Catch2WithMain)

    # set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
    # set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

