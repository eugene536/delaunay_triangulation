#ifndef CIRCULAR_LIST_H
#define CIRCULAR_LIST_H
#include <list>
#include <cassert>
#include <iostream>

namespace geometry {

const int MAX_MEM = 1e8;
int mpos = 0;
char mem[MAX_MEM];

template <typename T>
struct Node {
    Node(T value)
        : value_(value)
    {}

    Node(T value, Node* prev, Node* next)
        : value_(value)
        , prev_(prev)
        , next(next)
    {}

    void * operator new ( size_t n ) {
      char *res = mem + mpos;
      mpos += n;
      assert(mpos <= MAX_MEM);
      return (void *)res;
    }

    void operator delete (void *) noexcept { }

    T value_;
    Node<T>* prev_;
    Node<T>* next;
};

template <typename T>
class CircularList {
public:
    struct iterator;

public:
    CircularList() 
        : head_(nullptr)
        , tail_(nullptr)
        , size_(0)
    {}

    ~CircularList() {
        if (head_ == nullptr) return;
        for (size_t i = 0; i < size_ - 1; ++i) {
            head_ = head_->next;
            delete head_->prev_;
        }
        delete head_;
    }

    void push_back(T value) {
        insert_after(iterator(tail_), value);
    }

    void erase(iterator&& it) {
        Node<T>* cur = it.node();
        it.invalidate();
        Node<T>* next = cur->next;
        if (cur != cur->prev_) {
            cur->prev_->next = cur->next;
            cur->next->prev_ = cur->prev_;
        }

        if (head_ == cur)
            head_ = next;
        if (tail_ == cur)
            tail_ = next->prev_;
        if (head_ == tail_) 
            head_ = tail_ = nullptr;

        delete cur;

        size_--;
    }

    void insert_before(const iterator& it, const T& value) {
        Node<T>* cur = it.node();
        Node<T>* ver = new Node<T>(value, cur->prev_, cur);
        cur->prev_->next = ver;
        cur->prev_ = ver;

        if (cur == head_) {
            head_ = ver;
        }
        size_++;
    }

    void insert_after(const iterator& it, const T& value) {
        Node<T>* cur = it.node();
        Node<T>* ver = new Node<T>(value);

        if (head_ == nullptr) {
            cur = head_ = tail_ = ver;
            cur->next = cur;
            cur->prev_ = cur;
        } else {
            ver->next = cur->next;
            ver->prev_ = cur;

            cur->next->prev_ = ver;
            cur->next = ver;
        }

        if (cur == tail_) {
            tail_ = ver;
        }
        size_++;
    }

    iterator begin() const {
        return iterator(head_);
    }

    iterator end() const {
        return iterator(tail_);
    }

    size_t size() const {
        return size_;
    }

private:
    Node<T>* head_;
    Node<T>* tail_;
    size_t size_;
};

template<typename T>
struct CircularList<T>::iterator {
public:
    iterator(Node<T>* cur)
        : cur_(cur)
        , valid_(true)
    {}

    iterator& operator++() {
        assert(valid_);
        cur_ = cur_->next;
        return *this;
    }

    iterator operator++(int) {
        assert(valid_);
        iterator res(*this);
        ++(*this);
        return res;
    }

    iterator operator+(size_t cnt) const {
        assert(valid_);
        iterator res(*this);
        for (size_t i = 0; i < cnt; ++i, ++res);
        return res;
    }

    iterator& operator--() {
        assert(valid_);
        cur_ = cur_->prev_;
        return *this;
    }

    iterator operator-(size_t cnt) const {
        assert(valid_);
        iterator res(*this);
        for (size_t i = 0; i < cnt; ++i, --res);
        return res;
    }

    T& operator*() const {
        assert(valid_);
        return cur_->value_;
    }

    T* operator->() const {
        assert(valid_);
        return &cur_->value_;
    }

    bool operator==(const iterator& oth) const {
        return cur_ == oth.cur_;
    }

    bool operator!=(const iterator& oth) const {
        return cur_ != oth.cur_;
    }

private:
    void invalidate() {
        valid_ = false;
    }

    Node<T>* node() const {
        assert(valid_);
        return cur_;
    }
    
private:
    Node<T>* cur_;
    bool valid_;

private:
    template<typename Tp>
    friend class CircularList;
};

} // namespace geometry


#endif //CIRCULAR_LIST_H
