#pragma once

#include <utility>
#include <functional>
#include <cstddef>
#include "string.hpp"

namespace my {

class Map {
    struct Node;
public:
    using key_type = int;
    using mapped_type = int;
    using value_type = std::pair<key_type, mapped_type>;
    using size_type = size_t;

    constexpr static auto cmp = std::less< key_type >{};
    
public:
    Map();
    Map(const Map&);
    Map(Map&&);

    Map& operator=(const Map&);
    Map& operator=(Map&&);

    ~Map();
public:
    class iterator {
        friend class Map;
        explicit iterator(Node*);
    public:
        iterator& operator++();
        iterator operator++(int);
        bool operator==(const iterator&) const = default;
        value_type& operator*() const;
        
    private:
        static Node* descend_left(Node*);
     private:
        Node* mNode;
    };
public:
    mapped_type& operator[](const key_type&);
    const mapped_type& operator[](const key_type&) const;

    size_type count(const key_type&);
    size_type size() const;

    void clear();

    iterator find(const key_type&);
    iterator begin();
    iterator end();

    size_type erase(const key_type&);
    iterator erase(iterator);

private:
    struct Node {
        value_type value;
        Node* left = nullptr;
        Node* right = nullptr;
        Node* parent = nullptr;
    };

private:
    void recursive_erase(Node*);
    static size_type copy(const Node*, Node*&);
    Node* find_impl(const key_type&) const;
    
private:
    size_type mSize = 0;
    Node* mRoot = nullptr;
};

}// namespace my end