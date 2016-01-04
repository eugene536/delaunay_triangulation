#ifndef CIRCLE_H
#define CIRCLE_H
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

template<typename Tp> 
class Circle {
private:
    typedef Point<Tp, 2> Pnt;
    typedef Vector<Tp, 2> Vec;

public:
    Circle(Pnt a, Pnt b, Pnt c) {
        Pnt mid_ac = Segment<Tp>(a, c).Middle();
        Pnt mid_bc = Segment<Tp>(b, c).Middle();

        Vec norm_ac(Vec(a, c).GetNormal());
        Vec norm_bc(Vec(b, c).GetNormal());

        Tp param = (Vec(mid_ac).Cross(norm_ac) - Vec(mid_bc).Cross(norm_ac)) / norm_bc.Cross(norm_ac);

        center_ = Vec(mid_bc) + norm_bc * param;
        radius_ = a.Distance(center_);
    }

    Circle(Pnt a, Pnt b) {
        center_ = Segment<Tp>(a, b).Middle();
        radius_ = b.Distance(a) / 2;
    }

    Circle(Pnt a) {
        center_ = a;
        radius_ = 0;
    }

    bool Inside(const Pnt& pnt) {
        return center_.Distance(pnt) <= radius_;
    }

    Tp radius() const {
        return radius_;
    }

    Point<Tp> center() const {
        return center_;
    }

private:
    Point<Tp> center_;
    Tp radius_;

    template<typename U>
    friend std::ostream& operator<<(std::ostream& out, const Circle<U>& circle) {
        return out << circle.radius_ << "\n" << circle.center_;
    }
};


} // namespace geometry

#endif // CIRCLE_H
