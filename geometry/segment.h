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
public:
    const size_t sz = dim;

private:
    typedef Vector<Tp, dim> Vec;
    typedef Point<Tp, dim> Pnt;

public: 
    Segment (const Pnt& p1, const Pnt& p2) 
        : p1_(p1)
        , p2_(p2)
        , v_(p1, p2)
        , v_rev_(p2, p1)
    {}

    bool Inside(const Pnt& p) const {
        Vec p1_p(p1_, p);
        Vec p2_p(p2_, p);
        if (v_.Rotate(p1_p) == 0) 
            return v_.template DotProductSign<long long>(p1_p) * v_rev_.template DotProductSign<long long>(p2_p) >= 0;
        else
            return false;
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
};

} // namespace geometry

#endif // SEGMENT_H
