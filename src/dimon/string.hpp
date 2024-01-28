#pragma once

#include <cstddef>
#include <limits>
#include <iosfwd>
#include <compare>

namespace my
{

class String {
public:
    using value_type = char;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = value_type*;
    using const_iterator = const value_type*;
    using size_type = size_t;

public:
    constexpr static auto npos = std::numeric_limits< size_type >::max();

public:
    ~String();

    [[nodiscard]] String();
    [[nodiscard]] String(const char* const string, const size_type size);

    [[nodiscard]] String(const String&);
    [[nodiscard]] String(String&&);

    [[nodiscard]] String(const char* string);

    String& operator=(const String&);
    String& operator=(String&&);

    [[nodiscard]] String operator+(const String&) const;
    String& operator+=(const String&);

    [[nodiscard]] size_type find(const value_type needle) const;
    [[nodiscard]] size_type find(const String &needle) const;

    // from - откуда брать подстроку
    // sz - размер подстроки [from, from + sz], если from + sz > size() то до самого конца
    [[nodiscard]] String substr(size_type from, size_type sz = npos);
    String& insert(size_type index, const String &what);

    [[nodiscard]] std::strong_ordering operator<=>(const String&) const;
    [[nodiscard]] bool operator==(const String& rhs) const {
        return (*this <=> rhs) == std::strong_ordering::equal;
    }
    
    [[nodiscard]] iterator begin() { return mStr; }
    [[nodiscard]] iterator end() {return mStr + mSize; }
    [[nodiscard]] const_iterator begin() const { return mStr; }
    [[nodiscard]] const_iterator end() const { return mStr + mSize; }

    [[nodiscard]] const value_type* data() const { return mStr; }
    [[nodiscard]] value_type* data() { return mStr; }
    [[nodiscard]] size_type size() const { return mSize; }

private:
    value_type* mStr;
    size_type mSize;
    size_type mCapacity;
};

} // namespace my

std::ostream& operator<<(std::ostream&, const my::String&);



