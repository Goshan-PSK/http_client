#pragma once

#include <cstddef>
#include <iterator>
#include <utility>

namespace my {

class LinkedList {
    struct Node {
        int data;
        Node* next;
        Node* prev;
    };

public:
    using value_type = int;
    using size_type = size_t;
    
    template< typename T >
    class IteratorBase {
        friend class LinkedList;
        
        template< typename >
        friend class iterator;
    protected:
        IteratorBase(T item)
            : mItem(item) {
        }

    public:
        using difference_type = std::ptrdiff_t;
        using value_type = std::remove_reference_t< decltype(std::declval< T >()->data) >;
        using iterator_category = std::forward_iterator_tag;

    public:
        IteratorBase() = default;

        decltype(auto) operator*() const { return mItem->data; }

        template< typename OT >
        bool operator==(const IteratorBase< OT >& rhs) const {
            return mItem == rhs.mItem;
        }

        IteratorBase& operator++() {
            mItem = mItem->next;
            return *this;
        }

        IteratorBase operator++(int) {
            auto r = *this;
            ++(*this);
            return r;
        }

    private:
        T mItem = nullptr;
    };

    using iterator = IteratorBase< Node* >;
    using const_iterator = IteratorBase< const Node* >;

public:
    ~LinkedList();

    LinkedList();
    LinkedList(const LinkedList& rhs);
    LinkedList(LinkedList&& rhs);

    LinkedList& operator=(const LinkedList& rhs);
    LinkedList& operator=(LinkedList&& rhs);

public:
    void push_front(const value_type& data);
    void push_back(const value_type& data);

    void clear();

    iterator insert(iterator it, value_type& data);
    iterator erase(iterator it);

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    size_type size() const;
    
private:
    Node* mHead;
    Node* mTail;
    size_type mSize;
};

} // Namespace my end;