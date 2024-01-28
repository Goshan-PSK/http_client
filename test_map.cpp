#include <dimon/map.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace my;

TEST_CASE ("Dobavil 3 proveril size") {
    Map map;
    map[4], map[6], map[7], map[8], map[12], map[3], map[5];
    REQUIRE(map.size() == 7);

    WHEN("Clear") {
        map.clear();
        REQUIRE(map.size() == 0);
    }

    WHEN("Erase"){
        const auto checkExists = [](Map& map, const Map::key_type key, const bool exists) {
            const Map::iterator it = map.find(key);
            const bool realExists = it != map.end();

            REQUIRE(realExists == exists);
            if (realExists) {
                REQUIRE((*it).second == key);
                REQUIRE((*it).first == key);
            }
            REQUIRE(map.count(key) == exists);
        };
        
        for (int i = 0; i < 15; ++i) {
            map[i] = i;
            checkExists(map, i, true);
         
        }
        for (int i = 0; i < 15; ++i) {
            checkExists(map, i, true);
         
        }
        for (int i = 0; i < 15; ++i) {
            checkExists(map, i, true);
            REQUIRE(map.erase(i) == 1);
            REQUIRE(map.erase(i) == 0);
            checkExists(map, i, false);
        }
    }
    WHEN ("Map[3]") {
        map.clear();
        for (int i = 0; i < 10; ++i) {
            map[i] = i;
        }
        for (int i = 0; i < 10; ++i) {
            REQUIRE(map[i] == i);
        }
    }
    Map cop;
    cop = map;

    WHEN ("copy contryctor") {
        REQUIRE (cop.size() == map.size());
        for (Map::iterator l = map.begin(), r = cop.begin(); l != map.end(); ++l, ++r) {
            REQUIRE(*l == *r);
        }
    }

     WHEN ("move contryctor") {
        Map mcop;
        mcop = std::move(map);
        REQUIRE (map.size() == 0);
        REQUIRE (map.begin() == map.end());
        REQUIRE(mcop.size() == cop.size());
        REQUIRE (std::equal(mcop.begin(),mcop.end(), cop.begin()));
    }
}