#ifndef POINT_H
#define POINT_H
#include <iostream>
#include <vector>
#include <tuple>
#include <functional>
#include <cassert>
#include <string.h>
#include <cmath>

template<typename Tp, size_t dim> 
class Vector;

template<typename Tp, size_t dim = 2> 
class Point {
public:
    const size_t sz = dim;
    typedef Point<Tp, dim> Pnt;

public: 
    Point() 
    {
        memset(coordinates_, 0, sizeof(coordinates_));
    }

    template<class...CoordinateType>
    Point(CoordinateType ... coordinates) 
    {
        static_assert(sizeof...(CoordinateType) == dim, 
                "count of coordinates must be equal to count of dimensions of Point");
        Init(0, coordinates...);
    }

    Point(const Pnt& oth) 
    {
        *this = oth;
    }

    // sometimes you can't calculate `Distance`
    // due to `sqrt` works with not all types (e.g. `mpq_class` doesn't support `sqrt`)
    // use `Distance2` in this case, which returns Distance ^ 2 and you'll need calculate `sqrt` by hands
    // or you can use `Distance2` for perfomance reasons, because `sqrt` is slow =)
    double Distance(const Pnt& oth) {
        Tp sum = 0;
        for (size_t i = 0; i < dim; ++i) {
            Tp diff = coordinates_[i] - oth.coordinates_[i]; 
            sum += diff * diff;
        }

        return sqrt(static_cast<double>(sum));
    }

    // Calculates `Distance` ^ 2
    Tp Distance2(const Pnt& oth) {
        Tp sum = 0;
        for (size_t i = 0; i < dim; ++i) {
            Tp diff = coordinates_[i] - oth.coordinates_[i]; 
            sum += diff * diff;
        }

        return sum;
    }

    Pnt& operator=(const Pnt& oth) {
        std::copy(oth.coordinates_, oth.coordinates_ + sz, coordinates_);
        return *this;
    }

    bool operator<(const Point<Tp, dim>& oth) const {
        for (size_t i = 0; i < sz; ++i) {
            if (coordinates_[i] < oth.coordinates_[i]) {
                return true;
            } else if (coordinates_[i] > oth.coordinates_[i]) {
                return false;
            }
        }

        return false;
    }

    bool operator==(const Point<Tp, dim>& oth) const {
        for (size_t i = 0; i < sz; ++i) {
            if (coordinates_[i] != oth.coordinates_[i]) {
                return false;
            }
        }
        return true;
    }

private:
    template<class FirstCoordinateType, class...CoordinateType>
    void Init(int lev, FirstCoordinateType first, CoordinateType...coordinates) {
        static_assert(std::is_convertible<FirstCoordinateType, Tp>::value,
                "types must be convertible to Tp");

        coordinates_[lev] = static_cast<Tp>(first);
        Init(lev + 1, coordinates...);
    }

    void Init(int lev) {
        assert(lev == dim);
    }


private:
    Tp coordinates_[dim];

public:
    template<typename T, size_t d>
    friend void swap(Point<T, d>& first, Point<T, d>& second) {
        using std::swap;
        for (size_t i = 0; i < d; ++i) {
            swap(first.coordinates_[i], second.coordinates_[i]);
        }
    }

private:
    friend class Vector<Tp, dim>;

    template<typename T, size_t d>
    friend std::ostream& operator << (std::ostream& out, const Point<T, d>& point);

    template<typename T, size_t d>
    friend std::istream& operator >> (std::istream& in, Point<T, d>& point);
};

template<typename Tp, size_t dim = 2>
std::ostream& operator << (std::ostream& out, const Point<Tp, dim>& point) {
    for (const Tp& x: point.coordinates_) {
        out << x << " ";
    }
    return out;
}

template<typename Tp, size_t dim = 2>
std::istream& operator >> (std::istream& in, Point<Tp, dim>& point) {
    Tp x;
    for (size_t i = 0; i < dim; ++i) {
        in >> x;
        point.coordinates_[i] = x;
    }

    return in;
}

#endif // POINT_H
