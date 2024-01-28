#include <iostream>
#include <cstdlib>

#include <dimon/animals.hpp>

using namespace my;

void print(const Animal* animal) {
     Vector< const Animal* > currGen;
     Vector< const Animal* > nextGen;

    std::copy(
        animal->children().begin(),
        animal->children().end(),
        std::back_inserter(currGen)
    );
    
    while (currGen.size() > 0) {
        for (const auto a : currGen) {
            std::cout << a->name() << "(" << a->generation() << ")" << " | ";
        }
        std::cout << std::endl;

        for (const auto current : currGen) {
            std::copy(
                current->children().begin(),
                current->children().end(),
                std::back_inserter(nextGen)
            );
        }
        currGen.clear();
        std::copy(
            nextGen.begin(),
            nextGen.end(),
            std::back_inserter(currGen)
        );
        nextGen.clear();
    } 
}
    
int main() {
    std::srand(0);

    Vector< Animal* > farm;

    farm.push_back(new Cat(nullptr, nullptr));
    farm.push_back(new Dog(nullptr, nullptr));
    farm.push_back(new Cow(nullptr, nullptr));

    for (int i = 0; i < 100; ++i) {
        const auto father = farm[std::rand() % farm.size()];
        const auto mother = farm[std::rand() % farm.size()];
        if (const auto child = mother->beFuckedBy(father); child != nullptr) {
            farm.push_back(child);
        }
       //std::cout << i << ' ' << father->name() << ' ' << mother->name() << std::endl;
    }
    print(farm[1]);
    std::cout << farm[0]->generation() << " " << farm[3]->generation() << std::endl;
    return 0;
}