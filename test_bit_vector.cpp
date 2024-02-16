#include <dimon/vector.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>

using namespace my;
TEST_CASE ("dobavit 15 elementov") {
    Vector <bool> vec;
    const size_t repetCount = 150;
    for (int i = 0; i < repetCount; ++i) {
        vec.push_back((i & 1));
        REQUIRE(vec[i] == (i & 1));
    }
    for (int i = 0; i < repetCount; ++i) {
        REQUIRE(vec[i] == (i & 1));
    }
    const size_t cap = vec.capacity();
    REQUIRE(vec.size() == repetCount);

    WHEN ("pop_back") {
        vec.pop_back();
        REQUIRE(vec.size() == repetCount - 1);
        REQUIRE(vec.capacity() == cap);
    }

    WHEN ("erase") {

        for (int i = 0; i < 8; ++i) {
            std::cout << vec[i] << " ";
        }
        std::cout << std::endl;
        for (Vector< bool >::iterator it = vec.begin(); it != vec.end();) {

            if (!*it) {
                it = vec.erase(it);
                if (it != vec.end()) {
                    REQUIRE(*it == true);
                }
            }

            else {
                ++it;
            }


        }
        std::cout << "POSLE ERAST" << std::endl;
        for (int i = 0; i < 8; ++i) {
            std::cout << vec[i] << " ";
        }

        size_t bit_index = 0;
        for (Vector< bool >::iterator it = vec.begin(); it != vec.end(); ++it) {
            if (*it) {
                it = vec.insert(it, true);
                CAPTURE(bit_index);
                REQUIRE (*it == true);
                ++it;
            }
            bit_index++;
        }
        std::cout << std::endl << "POSLE insert" << std::endl;
        for (int i = 0; i < 8; ++i) {
            std::cout << vec[i] << " ";
        }
        std::cout<<std::endl;
    }
}