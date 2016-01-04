#ifndef UTILITIES_H
#define UTILITIES_H
#include "polygon.h"
#include <limits>

namespace geometry {

template<typename Tp>
using Vec = Vector<Tp, 2>;

template<typename Tp>
double Distance(const Polygon<Tp, 2>& poly1, const Polygon<Tp, 2>& poly2) {
    using namespace std;

    size_t i = 0;
    size_t j = 0;

    double min_dist = numeric_limits<double>::max();

    for (size_t k = 0; k < max(poly1.Size(), poly2.Size()) * 4; ++k) {

        size_t ni = (i + 1) % poly1.Size();
        size_t nj = (j + 1) % poly2.Size();

        min_dist = min(min_dist, poly1[i].Distance(Segment<Tp>(poly2[j], poly2[nj])));
        min_dist = min(min_dist, poly2[j].Distance(Segment<Tp>(poly1[i], poly1[ni])));

        if (Vec<Tp>(poly2[j], poly2[nj]).Rotate(Vec<Tp>(poly1[i], poly1[ni])) >= 0)
            j = nj;
        else
            i = ni;
    }
    assert(min_dist !=  numeric_limits<double>::max());

    return min_dist;
}

}

#endif // UTILITIES_H 
