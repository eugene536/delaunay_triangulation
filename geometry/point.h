#ifndef POINT_H
#define POINT_H
#include <iostream>
#include <vector>
#include <tuple>
#include <functional>
#include <cassert>
#include <string.h>

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
        init(0, coordinates...);
    }

    template<class FirstCoordinateType, class...CoordinateType>
    void init(int lev, FirstCoordinateType first, CoordinateType...coordinates) {
        static_assert(std::is_same<FirstCoordinateType, Tp>::value,
                "types must be equal to Tp");

        using namespace std;
        coordinates_[lev] = first;
        init(lev + 1, coordinates...);
    }

    void init(int lev) {
        assert(lev == dim);
    }

    //Point(const std::vector<Tp>& coordinates) {
        //assert(coordinates.size() == dim);
        //coordinates_ = coordinates;
    //}

    Point(const Pnt& oth) 
    {
        *this = oth;
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
    Tp coordinates_[dim];

private:
    friend class Vector<Tp, dim>;

    template<typename T, size_t d>
    friend std::ostream& operator << (std::ostream& out, const Point<T, d>& point);

    template<typename T, size_t d>
    friend std::istream& operator >> (std::istream& in, Point<T, d>& point);
};

template<typename Tp, size_t dim = 2>
std::ostream& operator << (std::ostream& out, const Point<Tp, dim>& point) {
    //out << "p(";
    if (point.sz > 0) {
        for (size_t i = 0; i < point.sz - 1; ++i) {
            out << point.coordinates_[i] << " ";
            //out << point.coordinates_[i] << ", ";
        }
        out << point.coordinates_[point.sz - 1];
    }
    return out;
    //return out << ")";
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
