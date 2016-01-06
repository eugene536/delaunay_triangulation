#ifndef SEGMENT_H
#define SEGMENT_H
#include <iostream>
#include <vector>
#include <tuple>
#include <functional>
#include <cmath>
#include <cassert>
#include "point.h"
#include "vector.h"

namespace geometry {

template<typename Tp, size_t dim = 2> 
class Segment {
private:
    typedef Vector<Tp, dim> Vec;
    typedef Point<Tp, dim> Pnt;

public: 
    Segment() {}

    Segment (const Pnt& p1, const Pnt& p2) 
        : p1_(p1)
        , p2_(p2)
        , v_(p1, p2)
        , v_rev_(p2, p1)
    {}

    void Reorder() {
        if (p2_ < p1_) {
            std::swap(p1_, p2_);
            std::swap(v_, v_rev_);
        }
    }

    bool Inside(const Pnt& p) const {
        Vec p1_p(p1_, p);
        Vec p2_p(p2_, p);
        if (v_.Rotate(p1_p) == 0) 
            return v_.template DotProductSign<long long>(p1_p) * v_rev_.template DotProductSign<long long>(p2_p) >= 0;
        else
            return false;
    }

    bool Intersected(const Segment<Tp>& oth) const {
        int f1 = v_.Rotate(Vec(p1_, oth.p1_));
        int f2 = v_.Rotate(Vec(p1_, oth.p2_));
        int f3 = oth.v_.Rotate(Vec(oth.p1_, p1_));
        int f4 = oth.v_.Rotate(Vec(oth.p1_, p2_));
        if (f1 == 0 && f2 == 0 && f3 == 0 && f4 == 0)
            return oth.Inside(p1_) || oth.Inside(p2_) || Inside(oth.p1_) || Inside(oth.p2_);
        else 
            return f1 * f2 <= 0 && f3 * f4 <= 0;
    }

    Pnt Middle() const {
        return p1_ + Pnt(v_) / 2;
    }

private:
    Pnt p1_, p2_;
    Vec v_, v_rev_;

private:

    template<typename, size_t>
    friend class Point;

    template<typename U>
    friend std::istream& operator>>(std::istream& in, Segment<U>& segment) {
        in >> segment.p1_ >> segment.p2_;
        segment.v_ = Vector<U>(segment.p1_, segment.p2_);
        segment.v_rev_ = Vector<U>(segment.p2_, segment.p1_);

        return in;
    }

    template<typename U>
    friend std::ostream& operator<<(std::ostream& out, const Segment<U>& segment) {
        return out << "SEGMENT: " << segment.p1_ << " " << segment.p2_;
    }

    template<typename U>
    friend std::pair<int, int> FindIntersection(std::vector<Segment<U>>& segments);
};

} // namespace geometry

#endif // SEGMENT_H
