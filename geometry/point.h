#ifndef POINT_H
#define POINT_H
#include <iostream>
#include <vector>
#include <tuple>
#include <functional>

template<typename Tp, size_t dim> 
class Vector;

template<typename Tp, size_t dim = 2> 
class Point {
public: 
    Point(){}

    template<class...CoordinateType>
    Point(CoordinateType ... coordinates) {
        static_assert(sizeof...(CoordinateType) == dim, 
                "count of coordinates must be equal to count of dimensions of Point");
        coordinates_ = {static_cast<Tp>(coordinates)...};
    }

    Point(const std::vector<Tp>& coordinates) {
        assert(coordinates.size() == dim);
        coordinates_ = coordinates;
    }

private:
    std::vector<Tp> coordinates_;

private:
    friend class Vector<Tp, dim>;

    template<typename T, size_t d>
    friend std::ostream& operator << (std::ostream& out, const Point<T, d>& point);

    template<typename T, size_t d>
    friend std::istream& operator >> (std::istream& in, Point<T, d>& point);
};

template<typename Tp, size_t dim = 2>
std::ostream& operator << (std::ostream& out, const Point<Tp, dim>& point) {
    out << "p(";
    if (point.coordinates_.size() > 0) {
        for (size_t i = 0; i < point.coordinates_.size() - 1; ++i) {
            out << point.coordinates_[i] << ", ";
        }
        out << point.coordinates_.back();
    }
    return out << ")";
}

template<typename Tp, size_t dim = 2>
std::istream& operator >> (std::istream& in, Point<Tp, dim>& point) {
    point.coordinates_.clear();
    Tp x;
    for (size_t i = 0; i < dim; ++i) {
        in >> x;
        point.coordinates_.push_back(x);
    }

    return in;
}

#endif // POINT_H
