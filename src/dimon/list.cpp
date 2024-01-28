#include "list.hpp"

using namespace my;

LinkedList& LinkedList::operator=(const LinkedList& rhs) {
    Node* left = mHead;
    Node* right = rhs.mHead;
    
    for (; left != nullptr && right != nullptr; left = left->next, right = right->next) {
        left->data = right->data;
    }

    std::copy(
        const_iterator(right),
        rhs.end(),
        std::back_inserter(*this)
    );

    if (left != nullptr) {
        mTail = left->prev;
        
        if (mTail != nullptr) {
            mTail->next = nullptr;
        }

        for (Node* next; left != nullptr; left = next) {
            next = left->next;
            delete left;
        }
    }

    if (rhs.mSize == 0) {
        mHead = nullptr;
    }
    mSize = rhs.mSize;
    return *this;
}

LinkedList& LinkedList::operator=(LinkedList&& rhs) {
    clear();
    mSize = rhs.mSize;
    mHead = rhs.mHead;
    mTail = rhs.mTail;
    
    rhs.mSize = 0;
    rhs.mHead = rhs.mTail = nullptr;

    return *this;
}

LinkedList::LinkedList()
    : mHead{nullptr}
    , mTail{nullptr}
    , mSize{0}
{
}

LinkedList::LinkedList(const LinkedList& rhs) 
    : LinkedList()
       
{
    std::copy(rhs.begin(), rhs.end(), std::back_inserter(*this));
}

LinkedList::LinkedList(LinkedList&& rhs) 
    : mSize{rhs.size()}
    , mHead{rhs.mHead}
    , mTail{rhs.mTail}
{
    rhs.mHead = rhs.mTail = nullptr;
    rhs.mSize = 0;
}

LinkedList::~LinkedList() {
    clear();
}

void LinkedList::push_front(const value_type& data) {
    Node* ptr = new Node{data, mHead, nullptr};
    if (mHead != nullptr) {
        mHead->prev = ptr;
    }
    if (mTail == nullptr) {
        mTail = ptr;
    }
    mHead = ptr;
    mSize++;
}
    

void LinkedList::push_back(const value_type& data) {
    Node* ptr = new Node{data, nullptr, mTail};
    if (mTail != nullptr)
        mTail->next = ptr;

    if (mHead == nullptr)
        mHead = ptr;

    mTail = ptr;
    mSize++;
}

void LinkedList::clear() {
    auto it = begin();
    while (it != end()) {
        it = erase(it);
    }
}

LinkedList::iterator LinkedList::begin() {
    return iterator(mHead);
}

LinkedList::iterator LinkedList::end() {
    return iterator(nullptr);
}

LinkedList::const_iterator LinkedList::begin() const {
    return const_iterator(mHead);
}

LinkedList::const_iterator LinkedList::end() const {
    return const_iterator(nullptr);
}

LinkedList::iterator LinkedList::erase(iterator it) {
    auto item = it.mItem;
    if (item->next != nullptr) 
        item->next->prev = item->prev;
    else
        mTail = item->prev;
    
    if (item->prev != nullptr)
        item->prev->next = item->next;
    else
        mHead = item->next;

    mSize--;

    auto itNext = item->next;
    delete item;
    return itNext;
}

LinkedList::size_type LinkedList::size() const {
    return mSize;
}

 LinkedList::iterator LinkedList::insert(iterator it, value_type& data) {
    const auto item = it.mItem;
    if (item == nullptr) {
        push_back(data);
        return iterator(mTail);
    }

    if (item->prev == nullptr) {
        push_front(data);
        return iterator(mHead);
    }

    mSize++;

    Node* ptr = new Node{data, item, item->prev};
    item->prev->next = ptr;
    item->prev = ptr;
    return iterator(ptr);
}
