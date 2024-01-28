#pragma once

namespace my {

template< typename T >
Vector< T >::~Vector() {
    delete[] mData;
}
template< typename T >
void Vector< T >::push_back(value_type value) {
    if (capacity() == size()) {
        
        mCapacity += 16;
        auto buf = new value_type[mCapacity];
        for (size_type i = 0; i < mSize;++i) {
            buf[i] = mData[i];
        }
        delete[] mData;
        mData = buf;
    }
    mData[mSize++] = value;
}

template< typename T >
void Vector< T >::pop_back() {
    mSize--;
}

template< typename T >
auto Vector< T >::operator[](size_type idx) -> reference {
    return mData[idx];
}

template< typename T >
auto Vector< T >::operator[](size_type idx) const -> const_reference {
    return mData[idx];
}

template< typename T >
auto Vector< T >::begin() -> iterator {
    return mData;
}

template< typename T >
auto Vector< T >::end() -> iterator {
    return mData + mSize;
}

template< typename T >
auto Vector< T >::begin() const -> const_iterator {
    return mData;
}

template< typename T >
auto Vector< T >::end() const -> const_iterator {
    return mData + mSize;
}

template< typename T >
auto Vector< T >::size() const -> size_type { 
    return mSize; 
}

template< typename T >
auto Vector< T >::capacity() const -> size_type { 
    return mCapacity; 
}

template< typename T >
void Vector< T >::clear() {
    mSize = 0;
}

template< typename T >
auto Vector< T >::erase(iterator it) -> iterator{
    
   auto ret = it;
    for( ;it != end()-1; it++) {
        *it = *(it+1);
    }
    mSize--;
    return ret;
}

} // namespace my
