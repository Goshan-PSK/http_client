#include <iostream>
#include <cstring>
#include <numeric>
#include <random>

#include <dimon/vector.hpp>
#include <dimon/string.hpp>
#include <dimon/list.hpp>
#include <dimon/map.hpp>

using namespace my;

template<typename T>
auto& print_value(const T& value) {
    std::cout << value;
    return std::cout;
}

template<typename L, typename R>
auto& print_value(const std::pair<L, R> &value) {
    std::cout << value.first << ":" << value.second;
    return std::cout;
}

const auto print = [](auto &a, const auto where) {
    std::cout << where << " size:" << a.size() << ' ';
    if constexpr (requires { a.capacity(); })
        std::cout << "cap:" << a.capacity() << ' ';

    for (auto it = a.begin(); it != a.end(); ++it)
        print_value(*it) << " ";
    endl(std::cout);
};


int main() {
    Vector< int > a;

    for (int i = 0; i < 10; ++i)
        a.push_back(i);

    print(a, "Add 10 elements");
    
    for (int i = 0; i < 3; ++i)
        a.pop_back();
    
    print(a, "After erasing 3 elements");

    const auto afterErase = a.erase(a.begin() + 2);
    
    std::cout << "Erase returned: " << *afterErase << std::endl;
    print(a, "After erase");

    for(int i = 0; i < 16;++i) {
        a.push_back(i+10);
    }
    print(a,"realocate");

    std:: cout << "WORKING CLAASSS" << std::endl;
    String f("XUY PIZDA ZALYPA");
    
    String x ("STROKA KOTORAYA PREBAVLENA");

    std::cout << f + x << std::endl;
    
    std::cout <<"--------------------METOD FIND -------------------------------" << std::endl; 

    std::cout << f.find('Z') << " | " << f.find('w') << " | " << f.find("ZALYPA") << std::endl;
    
    std::cout << "--------------------METOD SUBSTR AND INSERT -------------------------" << std::endl;

    std::cout << f.substr(4,5) << " | " << f.insert(3," Her") << std::endl;

    std::cout
        << (String ("aaa") < String ("aaaa")) << ' '
        << (String ("aasssa") < String ("aaaa")) << ' '
        << (String ("aaa") == String ("aaa")) << ' '
        << (String ("aaa") > String ("aaaa")) << ' '
        << (String ("aaaa") > String ("aaa")) << ' '
        << (String ("aaa") != String ("aaa")) << ' '
        << std::endl;

    std::cout << "HACHAL TEST LISTA ---------" << std::endl;

    LinkedList l;
    for (int i = 5; i < 10; ++i) {
        l.push_back(i);
    }
    print(l, "push_back");
    for (int i = 4; i >= 0; --i) {
        l.push_front(i);
    }
    print(l, "push_front");

    LinkedList::iterator five = std::find(l.begin(), l.end(), 5);

    const auto six = l.erase(five);
    print(l, std::string("Posle ERASE ") + std::to_string(*six));

    for (auto it = l.begin(); it != l.end(); ) {
        if (*it % 2 != 0) {
            it = l.erase(it);
        }
        else {
            ++it;
        }
    }
    
    print(l,"TOLKO chetnie");

    for (auto it = l.begin(); it != l.end(); ) {
        if (*it % 2 == 0) {
            auto value = *it + 1;
            l.insert(++it, value);
            
        }
        else {
            ++it;
        }
    }
    print(l,"comeback");

    LinkedList newL(l);
    print(newL,"newL");

    l.clear();
    newL = l;
    print(newL,"after clear");

    std::cout<< "TESTIIIIIIIIIIIIIM MAAAAAAAAAAAAAP ELI PALI---------------" << std::endl;

    Map map;
    // auto keys = std::array{"Xyu", "Pizda", "Zalypa"};
    // for (int i = 0; i < keys.size(); i++) {
    //     map[keys[i]] = i + 1;
    //     std::cout << "insert " << keys[i]  << ":" << map.count(keys[i]) << std::endl;
    // }
    // for (const char* key : keys) {
    //     std::cout << key << "=" << map[key] << std::endl;
    // }

    // auto numbers = std::vector< int >(19, int());
    // std::iota(numbers.begin(), numbers.end(), 0);
    // std::random_device rd;
    // std::mt19937 g(rd());
    // std::shuffle(numbers.begin(), numbers.end(), g);

   map[11], map[7], map[6], map[9], map[8], map[10];

    print(map,"MAp");

    map.erase(6);
    print(map,"Posle Erase");
  
    
    return 0;
}