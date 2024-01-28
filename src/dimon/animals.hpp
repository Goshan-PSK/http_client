#pragma once

#include "string.hpp"
#include "vector.hpp"

namespace my
{

class Animal {
public:
    Animal(const Animal* father, const Animal* mother)
        : mFather(father)
        , mMother(mother)
    {  
    }

    virtual ~Animal() = default;

    virtual String name() const = 0;
    virtual String makeNoize() const = 0;

    const Animal* father() const { return mFather; }
    const Animal* mother() const { return mMother; }
    size_t generation() const {
        return g(this);
    }

    Animal* beFuckedBy(Animal* father) {
        if (this == father) 
            return nullptr;
            
        const auto child = giveBirth(father);
        mChildren.push_back(child);
        father->mChildren.push_back(child);
        return child;
    }

    const Vector< const Animal* > &children() const {
        return mChildren;
    }

private:
    static size_t g(const Animal *a) {
        if (a->mother() == nullptr && a->father() == nullptr){
            return 0;
        }    
        if (a->mother() == nullptr) {
            return g(a->father()) + 1;
        }
        if (a->father() == nullptr) {
            return g(a->mother()) + 1;
        }
        return std::max(g(a->father()), g(a->mother())) + 1;
    }
    
    virtual Animal* giveBirth(const Animal* father) = 0;

private:
    const Animal* mFather;
    const Animal* mMother; 
    Vector< const Animal* > mChildren;
};

class Cat : public Animal {
public:
    using Animal::Animal;

    String name() const override {
        return "Cat";
    }

    String makeNoize() const override {
        return "Meow";
    }

private:
    Animal* giveBirth(const Animal* father) override {
        return new Cat(father, this);
    }
};

class Cow : public Animal {
public:
    using Animal::Animal;

    String name() const override {
        return "Cow";
    }

    String makeNoize() const override {
        return "Moo";
    }

private:
    Animal* giveBirth(const Animal* father) override {
        return new Cow(father, this);
    }
};

class Dog : public Animal {
public:
    using Animal::Animal;

    String name() const override {
        return "Dog";
    }

    String makeNoize() const override {
        return "Woof";
    }

private:
    Animal* giveBirth(const Animal* father) override {
        return new Dog(father, this);
    }
};
} // namespace my
