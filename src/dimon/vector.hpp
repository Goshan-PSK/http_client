#pragma once

#include <cstddef>
#include <limits>

namespace my
{

template < typename T >
class Vector {
public:
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = value_type*;
    using const_iterator = const value_type*;
    using size_type = size_t;

public:
    ~Vector();
    Vector() = default;
    Vector(const Vector&) = delete;
    Vector(Vector&&) = delete;

    Vector& operator=(const Vector&) = delete;
    Vector& operator=(Vector&&) = delete;
   
    // Dobavit value v konec
    void push_back(value_type value);
    // Dropnut poslednii element
    void pop_back();

    void clear();

    // Udalit it vernut sleduushi za nim iterator
    iterator erase(iterator it);

    reference operator[](size_type idx);
    const_reference operator[](size_type idx) const;

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    size_type size() const;
    size_type capacity() const;

private:
    size_type mSize{0};
    size_type mCapacity{0};
    value_type* mData{new value_type[0]};
};

} // namespace my

#include "vector_impl.hpp"