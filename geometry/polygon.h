#ifndef POLYGON_H
#define POLYGON_H
#include <iostream>
#include <vector>
#include <tuple>
#include <functional>
#include <cmath>
#include <cassert>
#include <algorithm>
#include "point.h"
#include "vector.h"

template<typename Tp, size_t dim = 2> 
class Polygon {
private:
    typedef Polygon<Tp, dim> Poly;
    typedef Vector<Tp, dim> Vec;
    typedef Point<Tp, dim> Pnt;

public: 
    Polygon() {}

    template<class...PointType>
    Polygon(PointType ... points) {
        static_assert(sizeof...(PointType) >= 3,
                "count of points must be >= 3");
        points_ = {static_cast<Point<Tp, dim>>(points)...};
    }

    double Perimeter() const {
        assert(points_.size() >= 3);
        double res = 0;
        for (size_t i = 0; i < points_.size() - 1; ++i) {
            res += Vec(points_[i], points_[i + 1]).Length();            
        }
        res += Vec(points_.back(), points_[0]).Length();

        return res;
    }

    double Square() const {
        assert(points_.size() >= 3);
        double res = 0;
        for (size_t i = 1; i < points_.size() - 1; ++i) {
            Vec first(points_[0], points_[i]);
            Vec second(points_[0], points_[i + 1]);
            res += first.CrossProduct(second).Length() / 2;            
        }

        return res;
    }

     Poly ConvexHull() {
        assert(dim == 2);
        size_t min_id = std::distance(points_.begin(), 
                                     std::min_element(points_.begin(), points_.end()));

        if (min_id != 0) {
            std::swap(points_[min_id], points_[0]);
        }

        std::sort(points_.begin() + 1, points_.end(), [this] (const Pnt& a, const Pnt& b) -> bool {
            Vec first = Vec(points_[0], a);
            Vec second = Vec(points_[0], b);
            if (first.Rotate(second) == 0) {
                return first.Length() < second.Length();
            }

            return first.Rotate(second) > 0;
        });

        Poly res;
        std::vector<Pnt> & new_points = res.points_;
        for (size_t i = 0; i < points_.size(); ++i) {
            while (new_points.size() >= 2) {
                Vec first(new_points[new_points.size() - 2], new_points.back());
                Vec second(new_points.back(), points_[i]);
                if (first.Rotate(second) > 0)
                    break;
                new_points.pop_back();
            }
            new_points.push_back(points_[i]);
        }

        return res;
    }

    std::vector<std::tuple<Tp, Tp, Tp>> Triangulation() const {
        assert(points_.size() >= 3);
        
    }

private:
    std::vector<Point<Tp, dim>> points_;

private:
    template<typename T, size_t d>
    friend std::ostream& operator << (std::ostream& out, const Polygon<T, d>& polygon);

    template<typename T, size_t d>
    friend std::istream& operator >> (std::istream& in, Polygon<T, d>& polygon);
};

template<typename Tp, size_t dim = 2>
std::ostream& operator << (std::ostream& out, const Polygon<Tp, dim>& polygon) {
    //out << "poly{";
    out << polygon.points_.size() << std::endl;
    if (polygon.points_.size() > 0) {
        for (size_t i = 0; i < polygon.points_.size() - 1; ++i) {
            out << polygon.points_[i] << std::endl;
        }
        out << polygon.points_.back();
    }
    return out;
    //return out << "}";
}

template<typename Tp, size_t dim = 2>
std::istream& operator >> (std::istream& in, Polygon<Tp, dim>& polygon) {
    polygon.points_.clear();

    size_t sz;
    in >> sz;
    polygon.points_.reserve(sz);

    Point<Tp, dim> x;
    for (size_t i = 0; i < sz; ++i) {
        in >> x;
        polygon.points_.push_back(x);
    }

    return in;
}

#endif // POLYGON_H
