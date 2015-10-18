#ifndef CIRCULAR_LIST_H
#define CIRCULAR_LIST_H
#include <list>
#include <cassert>

template <typename ValueType, 
          typename BaseIterator>
class CircularConstIterator {
public:
    typedef std::ptrdiff_t difference_type;
    typedef ValueType value_type;
    typedef const ValueType* pointer;
    typedef const ValueType& reference;
    typedef std::bidirectional_iterator_tag iterator_category;

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

    CircularConstIterator operator+=(int cnt) const {
        for (int i = 0; i < cnt; ++i, ++*this);
        return *this;
    }

    CircularConstIterator operator+(int cnt) const {
        CircularConstIterator res(*this);
        return res += cnt;
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

    CircularConstIterator operator-=(int cnt) const {
        for (int i = 0; i < cnt; ++i, --*this);
        return *this;
    }

    CircularConstIterator operator-(int cnt) const {
        CircularConstIterator res(*this);
        return res -= cnt;
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

    operator BaseIterator() const {
        if (cur_ == end_) {
            return beg_;
        }
        return cur_;
    }

    bool operator==(const CircularConstIterator<ValueType, BaseIterator>& oth) const {
        return cur_ == oth.cur_;
    }

    bool operator!=(const CircularConstIterator<ValueType, BaseIterator>& oth) const {
        return cur_ != oth.cur_;
    }

public:
    mutable BaseIterator cur_;
    BaseIterator beg_;
    BaseIterator end_;
    const bool valid_;
};

template <typename ValueType, 
          typename BaseIterator,
          typename BaseConstIterator = BaseIterator>
class CircularIterator : public CircularConstIterator<ValueType, BaseIterator> {
public:
    CircularIterator(BaseIterator cur, BaseIterator beg, BaseIterator end)
        : CircularConstIterator<ValueType, BaseIterator>(cur, beg, end)
    {}

    explicit CircularIterator(BaseIterator cur)
        : CircularConstIterator<ValueType, BaseIterator>(cur)
    {}

    const CircularIterator& operator++() const {
        CircularConstIterator<ValueType, BaseIterator>::operator++();
        return *this;
    }

    CircularIterator operator++(int) const {
        CircularIterator prev(*this);
        ++(*this);
        return prev;
    }

    CircularIterator& operator+=(int cnt) const {
        for (int i = 0; i < cnt; ++i, ++*this);
        return *this;
    }

    CircularIterator operator+(int cnt) const {
        CircularIterator res(*this);
        return res += cnt;
    }

    const CircularIterator& operator--() const {
        CircularConstIterator<ValueType, BaseIterator>::operator--();
        return *this;
    }

    CircularIterator operator--(int) const {
        CircularIterator prev(*this);
        --(*this);
        return prev;
    }

    CircularIterator& operator-=(int cnt) const {
        for (int i = 0; i < cnt; ++i, --*this);
        return *this;
    }

    CircularIterator operator-(int cnt) const {
        CircularIterator res(*this);
        return res -= cnt;
    }


    ValueType& operator*() const {
        assert(this->valid_);
        if (this->cur_ == this->end_) {
            return *this->beg_;
        }
        return *this->cur_;
    }

    ValueType* operator->() const {
        assert(this->valid_);
        if (this->cur_ == this->end_) {
            return &(*this->beg_);
        }
        return &(*this->cur_);
    }

    operator BaseConstIterator() const {
        if (this->valid_ && this->cur_ == this->end_) {
            return static_cast<BaseConstIterator>(this->beg_);
        }
        return static_cast<BaseConstIterator>(this->cur_);
    }
};

template <typename T>
class CircularList : public std::list<T> {
public:
    typedef CircularIterator<T, typename std::list<T>::iterator, typename std::list<T>::const_iterator> iterator;
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
