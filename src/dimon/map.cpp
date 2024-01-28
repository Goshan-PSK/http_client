#include "map.hpp"
#include <stdexcept>
#include <cassert>
#include <iostream>
#include"vector.hpp"

using namespace my;

Map::Map() = default;
Map::Map(const Map& other) {
    *this = other;
}

Map::Map(Map&& other) {
    *this = std::move(other);
}

Map& Map::operator=(const Map& rhs) {
    clear();

    if (rhs.mRoot) {
        const auto sz = copy(rhs.mRoot, mRoot);
        mSize = rhs.mSize;
        assert(sz == mSize);
    }

    return *this;
}

Map& Map::operator=(Map&& rhs) {
    clear();

    mRoot = rhs.mRoot;
    mSize = rhs.mSize;
    rhs.mRoot = nullptr;
    rhs.mSize = 0;

    return *this;   
}

Map::~Map() {
    clear();
}

Map::mapped_type& Map::operator[](const key_type& key) {
    if (const auto node = find_impl(key)) {
        return node->value.second;
    }

    Node* node = new Node{{key, mapped_type()}};

    Node* cur = mRoot;
    Node* parent = nullptr;
    Node* Node::*source;
    while (cur != nullptr) {
        parent = cur;
        if (cmp(key, cur->value.first)) {
            source = &Node::left;
        }
        else {
            source = &Node::right;
        }
        cur = std::invoke(source, cur);
    }
    if (parent != nullptr) {
        std::invoke(source, parent) = node;
        node->parent = parent;
    }
    else {
        mRoot = node;
    }

    mSize++;

    return node->value.second;
}

const Map::mapped_type& Map::operator[](const key_type& key) const {
    if (const auto node = find_impl(key)) {
        return node->value.second;
    }
    throw std::runtime_error("invalid_key");
}

Map::size_type Map::size() const {
    return mSize;
}
void Map::clear() {
    for(iterator it = begin(); it != end();) {
       it = erase(it);
    }
     
    mSize = 0;
    mRoot = nullptr;
}

Map::Node* Map::find_impl(const key_type& key) const  {
    Node* cur = mRoot;
    while (cur != nullptr) {
        if (cmp(key, cur->value.first)) {
            cur = cur->left;
        }
        else if (cmp(cur->value.first, key)) {
            cur = cur->right;
        }
        else {
            return cur;
        }
    }
    return nullptr;
}

Map::iterator Map::find (const key_type& key) {
    return iterator(find_impl(key));
}

void Map::recursive_erase(Node* node) {
    if(!node) {
        return;
    }
    recursive_erase(node->left);
    recursive_erase(node->right);

    delete node;
}

Map::size_type Map::copy(const Node* from, Node*& to) {
    to = new Node{from->value};
    size_t r = 1;

    if (from->left != nullptr) {
        r += copy(from->left, to->left);
        to->left->parent = to;
    }
    if(from->right != nullptr) {
        r += copy(from->right, to->right);
        to->right->parent = to;
    }
    return r;
}

Map::size_type Map::count(const key_type& key) {
    return find_impl(key) ? 1 : 0;
}

Map::iterator& Map::iterator::operator++() {

    if (mNode->right) {
        mNode = descend_left(mNode->right);
    }
    else {
        while (mNode->parent && mNode == mNode->parent->right) {
            mNode = mNode->parent;
        }
        mNode = mNode->parent;
    }
    return *this;
}

Map::iterator Map::iterator::operator++(int) {
    iterator result = *this;
    ++(*this);
    return result;
}

Map::value_type& Map::iterator::operator*() const {
    return mNode->value;
}

Map::iterator::iterator(Node* node)
    :mNode{node}
{
}

Map::iterator Map::begin() {
    if (!mRoot) {
        return iterator(nullptr);
    }
    return iterator(iterator::descend_left(mRoot));
}

Map::Node* Map::iterator::descend_left(Node* cur) {
    while (cur->left != nullptr) {
        cur = cur->left;
    }
    return cur;
}

Map::iterator Map::end() {
    return iterator(nullptr);
}

Map::iterator Map::erase(iterator it) {

    const auto parentSide = [](Node* node)-> Node*& {
        if(node == node->parent->left) {
            return node->parent->left;
        }
        return node->parent->right;
    };

    Node* const node = it.mNode;

    if (!node->left && !node->right) {
        if(node->parent != nullptr) { 
            if(node == node->parent->left) {
                node->parent->left = nullptr;
            }
            else {
                node->parent->right = nullptr;
            }
        }
        else {
            mRoot = nullptr;
        }
    }

    else if (node->left && node->right) {
        Node* left = iterator::descend_left(node->right);
        node->left->parent = left;
        left->left = node->left;
        node->right->parent = node->parent; 
        if(node->parent == nullptr) {
            mRoot = node;
        }
        else {
            parentSide(node) = node->right;
        }

    }
    
    else {
        Node* side = node->left != nullptr ? node->left : node->right;
        side->parent = node->parent;
        if (node->parent) {
            parentSide(node) = side;
        }
        else {
            mRoot = side;
        }
    }
    mSize--;
    ++it;
    delete node;
    return it;
}

Map::size_type Map::erase(const key_type& key) {
    if (Node* tmp = find_impl(key)) {
        erase(iterator(tmp));
        return 1;
    }
    return 0;
}
