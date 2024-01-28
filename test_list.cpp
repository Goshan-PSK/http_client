#include <dimon/list.hpp>
#include <iostream>
#include <dimon/vector.hpp>

int main () {
    auto a = my::Vector< int >();
    for (auto it = a.begin(); it != a.end();) {
        if (*it % 2 == 0) {
            it = a.erase(it);
        }
        else {
            ++it;
        }
    }

}
