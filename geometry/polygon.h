#ifndef POLYGON_H
#define POLYGON_H
#include <iostream>
#include <vector>
#include <tuple>
#include <functional>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <list>
#include <iterator>
#include <unordered_map>
#include "point.h"
#include "vector.h"
#include "circular_list.h"
#include "segment.h"

namespace geometry {

template<bool... Types>
struct all_true;

template<bool... Types>
struct all_true<true, Types...> 
    : all_true<Types...>
{};

template<bool...Types>
struct all_true<false, Types...> 
    : std::false_type
{};

template<>
struct all_true<> 
    : std::true_type
{};

enum Location : int {
    INSIDE, BORDER, OUTSIDE
};

template<typename Tp, size_t dim = 2> 
class Polygon {
private:
    typedef Polygon<Tp, dim> Poly;
    typedef Vector<Tp, dim> Vec;
    typedef Point<Tp, dim> Pnt;
    typedef CircularList<std::pair<const Pnt*, size_t>> CircularPoints;
    typedef typename CircularPoints::iterator CPointsIterator;

public: 
    Polygon() 
        : sz_(0)
    {}

    explicit Polygon(size_t sz)
        : sz_(sz)
    {}
        
    template<class...PointType, typename = typename 
        std::enable_if<
            all_true<
                std::is_same<
                    typename std::decay<PointType>::type, Pnt
                >::value...
            >::value
        >::type
    >
    Polygon(const PointType& ... points) {
        static_assert(sizeof...(PointType) >= 3,
                "count of points must be >= 3");
        points_ = {points...};
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

    // returns vector of triples
    // count of result triangles = size of result vector / 3
    std::vector<Tp> Triangulation() const {
        using std::make_tuple;

        assert(points_.size() >= 3);
        std::vector<Tp> res;
        res.reserve(3 * (points_.size() - 2));

        CircularPoints points;
        for (size_t i = 0; i < points_.size(); ++i) {
            points.push_back(std::make_pair(&points_[i], i + 1));
        }

        CircularList<CPointsIterator> ears;
        size_t i = 0;
        for (auto it = points.begin(); i < points.size(); ++it, ++i) {
            if (IsEar(points, it)) {
                ears.push_back(it);
            }
        }

        if (points.size() > 3)
        for (auto it = ears.begin(); ears.size() >= 2;) {
            CPointsIterator cur_point = *it;
            res.push_back((cur_point - 1)->second);
            res.push_back(cur_point->second);
            res.push_back((cur_point + 1)->second);
            
            points.erase(--cur_point + 1);
            if (points.size() == 3) {
                break;
            }

            if (IsEar(points, cur_point)) {
                if (*(it - 1) != cur_point) {
                    ears.insert_before(it, cur_point);
                }
            } else if (*(it - 1) == cur_point) {
                ears.erase(it - 1);
            }

            if (IsEar(points, cur_point + 1)) {
                if (*(it + 1) != cur_point + 1) {
                    ears.insert_after(it, cur_point + 1);
                }
            } else if (*(it + 1) == cur_point + 1) {
                ears.erase(it + 1);
            }

            ears.erase(++it - 1);
        }

        assert(points.size() == 3);
        auto it = points.begin();
        res.push_back(it->second);
        res.push_back((it + 1)->second);
        res.push_back((it + 2)->second);

        return res;
    }

    Location CheckInside(const Pnt& p) {
        assert(dim == 2);

        points_.push_back(points_[0]);
        size_t cnt_intersections = 0;
        for (size_t i = 0; i < points_.size() - 1; ++i) {
            if (Segment<int>(points_[i], points_[i + 1]).Inside(p)) 
                return INSIDE;

            if (points_[i].y() == points_[i + 1].y()) 
                continue;

            double t2 = (p.y() - points_[i].y()) * 1.0 / (points_[i + 1].y() - points_[i].y());
            double t1 = points_[i].x() - p.x() + (points_[i + 1].x() - points_[i].x()) * t2;

            cnt_intersections += t2 >= 0 && t2 <= 1 && t1 >= 0 && p.y() != std::max(points_[i].y(), points_[i + 1].y());
        }
        points_.pop_back();

        return cnt_intersections % 2 == 0 ? OUTSIDE : INSIDE;
    }

    Location CheckConvexInside(const Pnt& p) {
        int l = 1;
        int r = static_cast<int>(points_.size()) - 1;

        Vec pv = Vec(points_[0], p);
        Vec lv = Vec(points_[0], points_[l]);
        Vec rv = Vec(points_[0], points_[r]);

        int lvr = lv.Rotate(pv);
        int rvr = rv.Rotate(pv);
        if (lvr * rvr > 0)
            return OUTSIDE;
        else if (lvr == 0) 
            return Segment<Tp>(points_[0], points_[l]).Inside(p) ? BORDER : OUTSIDE;
        else if (rvr == 0)
            return Segment<Tp>(points_[0], points_[r]).Inside(p) ? BORDER : OUTSIDE;

        while (r - l > 1) {
            int mid = (l + r) / 2;

            Vec mv(points_[0], points_[mid]);
            lv = Vec(points_[0], points_[l]);

            if (lv.Rotate(pv) * mv.Rotate(pv) <= 0)
                r = mid;
            else
                l = mid;
        }

        Vec lr(points_[l], points_[r]);
        Vec l0(points_[l], points_[0]);
        Vec lp(points_[l], p);

        if (lr.Rotate(lp) * l0.Rotate(lp) <= 0)
            return lr.Rotate(lp) == 0 ? BORDER : INSIDE;

        return OUTSIDE;
    }

private:
    bool IsEar(const CircularPoints& points, const CPointsIterator& cur_it) const
    {
        const Pnt& prev = *(cur_it - 1)->first;
        const Pnt& cur = *cur_it->first;
        const Pnt& next = *(cur_it + 1)->first;
        // if ear is convex
        if (Vec(prev, cur).Rotate(Vec(cur, next)) <= 0) {
            return false;
        }

        auto it = points.begin();
        do {
            auto point = *it;
            if (*point.first == prev || 
                *point.first == cur || 
                *point.first == next) 
            {
                continue;    
            }

            const Pnt& cur_p = *point.first;

            // point liy inside the triangle prev-cur-next
            // and therefore point cur isn't an ear
            if (Vec(next, prev).Rotate(Vec(next, cur_p)) >= 0 &&
                Vec(prev, cur).Rotate(Vec(prev, cur_p)) >= 0 &&
                Vec(cur, next).Rotate(Vec(cur, cur_p)) >= 0)
            {
                return false;
            }
        } while (points.end() != it++);

        return true;
    }

private:
    std::vector<Pnt> points_;
    size_t sz_;

private:
    template<typename T, size_t d>
    friend std::ostream& operator << (std::ostream& out, const Polygon<T, d>& polygon);

    template<typename T, size_t d>
    friend std::istream& operator >> (std::istream& in, Polygon<T, d>& polygon);
};

template<typename Tp, size_t dim = 2>
std::ostream& operator << (std::ostream& out, const Polygon<Tp, dim>& polygon) {
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

    if (polygon.sz_ == 0)
        in >> polygon.sz_;
    polygon.points_.reserve(polygon.sz_);

    Point<Tp, dim> x;
    for (size_t i = 0; i < polygon.sz_; ++i) {
        in >> x;
        polygon.points_.push_back(x);
    }

    return in;
}

} // namespace geometry

#endif // POLYGON_H
