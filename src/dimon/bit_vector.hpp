#pragma once

#include <algorithm>
#include <cstdint>
#include <climits>
#include "vector.hpp"

namespace  my
{

template<>
class Vector< bool > {
    using repr_type = uint8_t;
    constexpr static auto repr_bits = sizeof(repr_type) * CHAR_BIT;

public:
    using value_type = bool;
    using const_reference = const value_type&;
    using const_iterator = const value_type*;
    using size_type = size_t;

class reference {
public:
    reference(repr_type& data, size_t bit_number)
        : mData{data}
        , mBitNumber{bit_number}
    {
    }
public:
    operator value_type() const {
        return (mData >> mBitNumber) & 1;
    }

    reference& operator=(const value_type value) {
        if (value) {
            mData &= 1 << mBitNumber;
        }
        else {
            mData |= ~(1 << mBitNumber);
        }
        return *this;
    }
private:
    size_t mBitNumber;
    repr_type& mData;
};

class iterator {
public:
    friend class Vector<bool>;
    iterator(repr_type* Data, size_t index)
        : mData{Data}
        , mBitNumber{index}
    {
    }

    iterator& operator++() {
        mBitNumber++;
        return *this;
    }

    iterator operator++(int) {
        auto result = iterator(*this);
        mBitNumber++;
        return result;
    }

    bool operator==(const iterator& rhs) const {
        return mData == rhs.mData && mBitNumber == rhs.mBitNumber;

    }

    reference operator*() {
        return reference(mData[mBitNumber / repr_bits], mBitNumber % repr_bits);
    }
private:
    repr_type* mData;
    size_t mBitNumber;
};

public:
    ~Vector() {
        delete [] mData;
    }
    Vector() = default;
public:
    // Dobavit value v konec
    void push_back(const value_type value) {
        const size_t byte_number = mSize / repr_bits;
        const size_t bit_number = mSize % repr_bits;
        if (mSize == mCapacity) {
            mCapacity += 16 * repr_bits;
            repr_type * const newData = new repr_type[mCapacity / repr_bits];
            std::copy(mData, mData + byte_number, newData);
            delete [] mData;
            mData = newData;
        }
        if (value) {
            mData[byte_number] |= 1 << bit_number;
        }
        else {
            mData[byte_number] &= ~(1 << bit_number);
        }
        ++mSize;
    }
    // Dropnut poslednii element
    void pop_back() {
        --mSize;
    }

    void clear() {
        mSize = 0;
    }

    // Udalit it vernut sleduushi za nim iterator
    iterator erase(iterator it){
        const size_t byte_number = it.mBitNumber / repr_bits;
        const size_t bit_number = it.mBitNumber % repr_bits;
        size_t bigBit = repr_bits - 1;

        repr_type maskR = mData[byte_number] >> (bit_number + 1);
        repr_type maskL = mData[byte_number] << (repr_bits - bit_number);
        maskR = maskR << (byte_number + 1);
        maskL = maskL >> (repr_bits - bit_number - 1);

        mData[byte_number] = maskR & maskL;

        size_t size;
        if (mSize % repr_bits == 0) {
            size = mSize / repr_bits;
        }
        else {
            size = mSize / repr_bits + 1;
        }

        for (size_t i = byte_number + 1; i != size; ++i){
            repr_type GreatestBit = mData[i] & (1 << bigBit);
            mData[i-1] |= GreatestBit >> bigBit;
            mData[i] <<= 1;
        }

        return it;
    }

    reference operator[](size_type bit_number) {
        return reference(mData[mSize / repr_bits], mSize % repr_bits);
    }

    iterator begin() {
        return iterator(mData, 0);
    }
    iterator end() {
        return iterator(mData, mSize);
    }

    size_type size() const {
        return mSize;
    }
    size_type capacity() const {
    return mCapacity;
    }
private:

    size_type mSize{0};
    size_type mCapacity{0};
    repr_type* mData{new repr_type[0]};
};

} // namespace  my