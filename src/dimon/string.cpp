#include "string.hpp"

#include <iterator>
#include <iostream>
#include <algorithm>
    
using namespace my;

[[nodiscard]] size_t size(const char* const string) { // значение функции должно использоваться;
    auto it = string;
    while (*it)
        ++it;
    
    return std::distance(string, it);
}

String::String()
    : mStr(new value_type[0])
    , mSize(0)
    , mCapacity(0)
{
}

String::String(const String& string) 
    : String(string.data(), string.size())
{
}

String::String(String&& string) 
    : mStr(string.mStr)
    , mSize(string.mSize)
    , mCapacity(string.mCapacity)
    
{ 
    string.mStr = new value_type[0];
    string.mSize = string.mCapacity = 0;
}

String::String(const char* const string)
    : String(string, ::size(string))
{
}

String::String(const char* const string, const size_type size)
    : mSize(size)
    , mCapacity(mSize)
{
    if(mSize == 0) {
        mStr = new value_type[0];
    }
    else {
        mStr = new value_type[mSize + 1];
        std::copy(string, string + size, mStr);
        mStr[mSize] ='\0';
    }
}

String& String::operator=(const String& string) {
    if(string.mSize > mCapacity) {
        delete [] mStr;
        mStr = new value_type[string.mSize + 1];
        mCapacity = string.mSize;
    }
    mSize = string.mSize;
    std::copy(string.begin(), string.end(), begin());
    return *this;
}

String& String::operator=(String&& string){
    delete [] mStr;
    mStr = string.mStr;
    mSize = string.mSize;
    mCapacity = string.mCapacity;
    string.mStr = new value_type[0];
    string.mSize = string.mCapacity = 0;

    return *this;
}

String String::operator+(const String& string) const {
    String result = *this;
    return result += string;
}

String& String::operator+=(const String& string) {
    const auto newSize = mSize + string.mSize;
    if(newSize > mCapacity) {
        auto buf = new value_type[newSize + 1];
        std::copy(begin(), end(), buf);
        delete [] mStr;
        mStr = buf;
        mCapacity = newSize;
    }
    std::copy(string.begin(),string.end() ,mStr + mSize);
    mSize = newSize;

    return *this;
}

String::size_type String::find(const value_type needle) const {
    if (const auto it = std::find(begin(), end(), needle); it != end()) {
        return std::distance(begin(),it);
    }
    return npos;
}

String::size_type String::find(const String &string) const {
    const auto it = std::search(begin(), end(), string.begin(), string.end());
    if (it == end()) {
        return npos;
    }
    return std::distance(begin(), it);
}

String String::substr(size_type from, size_type sz) {
    if (from + sz > mSize || from + sz < from) {
        sz = mSize - from;
    }
    return String(mStr + from, sz);
}

String& String::insert(size_type index, const String &what) {
    if(what.mSize + mSize < mCapacity) {
        auto buf = new value_type[mSize + what.mSize + 1];
        const auto insertPlace = begin() + index;
        std::copy(begin(), insertPlace, buf);
        std::copy(insertPlace, end(), buf + what.mSize);
        delete[] mStr;
        mStr = buf;
        mCapacity = what.mSize + mSize;
    }
    else {
        std::copy_backward(mStr + index, mStr + mSize,mStr + mSize + what.mSize);
    }
    std::copy(what.begin(), what.end(), mStr + index);
    mSize = what.mSize + mSize;
    return *this;
}

std::strong_ordering String::operator<=>(const String& rhs) const {
    if (size() == rhs.size())
        return std::lexicographical_compare_three_way(
            begin(), end(),
            rhs.begin(), rhs.end()
        );
    return size() <=> rhs.size();
}

String::~String(){
    delete [] mStr;
}

std::ostream& operator<<(std::ostream& out, const String& s) {
    out.write(s.data(), s.size());
    return out;
}