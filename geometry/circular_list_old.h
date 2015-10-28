#ifndef CIRCULAR_LIST_H
#define CIRCULAR_LIST_H
#include <list>
#include <cassert>

template <typename T>
class CircularList;

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
    CircularConstIterator(CircularList<ValueType>* list, BaseIterator cur, BaseIterator end)
        : cur_(cur)
        , list_(list)
        , end_(end)
        , valid_(true)
    {}

    explicit CircularConstIterator(CircularList<ValueType>* list, BaseIterator cur)
        : cur_(cur)
        , valid_(false)
        , list_(list)
    {}

    const CircularConstIterator& operator++() const {
        assert(valid_);
        if (cur_ == end_) {
            cur_ = list_->begin();
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
        if (*this == list_->begin()) {
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
            return *list_->begin();
        }
        return *cur_;
    }

    const ValueType* operator->() const {
        assert(valid_);
        if (cur_ == end_) {
            return &(*list_->begin());
        }
        return &(*cur_);
    }

    operator BaseIterator() const {
        if (cur_ == end_) {
            return list_->begin();
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
    CircularList<ValueType>* list_;
    mutable BaseIterator cur_;
    BaseIterator end_;
    const bool valid_;
};

template <typename ValueType, 
          typename BaseIterator,
          typename BaseConstIterator = BaseIterator>
class CircularIterator : public CircularConstIterator<ValueType, BaseIterator> {
public:
    CircularIterator(CircularList<ValueType>* list, BaseIterator cur, BaseIterator end)
        : CircularConstIterator<ValueType, BaseIterator>(list, cur, end)
    {}

    explicit CircularIterator(CircularList<ValueType>* list, BaseIterator cur)
        : CircularConstIterator<ValueType, BaseIterator>(list, cur)
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
            return *this->list_->begin();
        }
        return *this->cur_;
    }

    ValueType* operator->() const {
        assert(this->valid_);
        if (this->cur_ == this->end_) {
            return &(*this->list_->begin());
        }
        return &(*this->list_->begin());
    }

    operator BaseConstIterator() const {
        if (this->valid_ && this->cur_ == this->end_) {
            return static_cast<BaseConstIterator>(this->list_->begin());
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
        return iterator(this, base::begin(), base::end());
    }

    const_iterator begin() const {
        return const_iterator(this, base::begin(), base::end());
    }

    const_iterator cbegin() const {
        return const_iterator(this, base::begin(), base::end());
    }

    iterator end() {
        return iterator(this, base::end());
    }

    const_iterator end() const {
        return const_iterator(this, base::end());
    }

    const_iterator cend() const {
        return const_iterator(this, base::end());
    }
};

#endif //CIRCULAR_LIST_H
