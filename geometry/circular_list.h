#ifndef CIRCULAR_LIST_H
#define CIRCULAR_LIST_H
#include <list>
#include <cassert>

template <typename ValueType, typename BaseIterator>
class CircularIterator {
public:
    CircularIterator(BaseIterator cur, BaseIterator beg, BaseIterator end)
        : cur_(cur)
        , beg_(beg)
        , end_(end)
        , valid_(true)
    {}

    explicit CircularIterator(BaseIterator cur)
        : cur_(cur)
        , valid_(false)
    {}

    CircularIterator& operator++() {
        assert(valid_);
        if (cur_ == end_) {
            cur_ = beg_;
        }
        cur_++;

        return *this;
    }

    CircularIterator operator++(int) {
        CircularIterator prev(*this);
        ++(*this);
        return prev;
    }

    CircularIterator& operator--() {
        assert(valid_);
        if (cur_ == beg_) {
            cur_ = end_;
        }
        cur_--;

        return *this;
    }

    CircularIterator operator--(int) {
        CircularIterator prev(*this);
        --(*this);

        return prev;
    }

    ValueType& operator*() {
        assert(valid_);
        if (cur_ == end_) {
            return *beg_;
        }
        return *cur_;
    }

    ValueType* operator->() {
        assert(valid_);
        if (cur_ == end_) {
            return &(*beg_);
        }
        return &(*cur_);
    }

    bool operator==(const CircularIterator<ValueType, BaseIterator>& oth) const {
        return cur_ == oth.cur_;
    }

    bool operator!=(const CircularIterator<ValueType, BaseIterator>& oth) const {
        return cur_ != oth.cur_;
    }

private:
    BaseIterator cur_;
    BaseIterator beg_;
    BaseIterator end_;
    const bool valid_;
};

template <typename ValueType, typename BaseIterator>
class CircularConstIterator {
public:
    CircularConstIterator(BaseIterator cur, BaseIterator beg, BaseIterator end)
        : cur_(cur)
        , beg_(beg)
        , end_(end)
        , valid_(true)
    {}

    explicit CircularConstIterator(BaseIterator cur)
        : cur_(cur)
        , valid_(false)
    {}

    const CircularConstIterator& operator++() const {
        assert(valid_);
        if (cur_ == end_) {
            cur_ = beg_;
        }
        cur_++;

        return *this;
    }

    CircularConstIterator operator++(int) const {
        CircularConstIterator prev(*this);
        ++(*this);
        return prev;
    }

    const CircularConstIterator& operator--() const {
        assert(valid_);
        if (cur_ == beg_) {
            cur_ = end_;
        }
        cur_--;

        return *this;
    }

    CircularConstIterator operator--(int) const {
        CircularConstIterator prev(*this);
        --(*this);

        return prev;
    }

    const ValueType& operator*() const {
        assert(valid_);
        if (cur_ == end_) {
            return *beg_;
        }
        return *cur_;
    }

    const ValueType* operator->() const {
        assert(valid_);
        if (cur_ == end_) {
            return &(*beg_);
        }
        return &(*cur_);
    }

    bool operator==(const CircularConstIterator<ValueType, BaseIterator>& oth) const {
        return cur_ == oth.cur_;
    }

    bool operator!=(const CircularConstIterator<ValueType, BaseIterator>& oth) const {
        return cur_ != oth.cur_;
    }

private:
    mutable BaseIterator cur_;
    BaseIterator beg_;
    BaseIterator end_;
    const bool valid_;
};

template <typename T>
class CircularList : public std::list<T> {
public:
    typedef CircularIterator<T, typename std::list<T>::iterator> iterator;
    typedef CircularConstIterator<T, typename std::list<T>::const_iterator> const_iterator;
    typedef std::list<T> base;

    iterator begin() {
        return iterator(base::begin(), base::begin(), base::end());
    }

    const_iterator begin() const {
        return const_iterator(base::begin(), base::begin(), base::end());
    }

    const_iterator cbegin() const {
        return const_iterator(base::begin(), base::begin(), base::end());
    }

    iterator end() {
        return iterator(base::end());
    }

    const_iterator end() const {
        return const_iterator(base::end());
    }

    const_iterator cend() const {
        return const_iterator(base::end());
    }
};

#endif //CIRCULAR_LIST_H
