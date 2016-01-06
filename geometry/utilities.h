#ifndef UTILITIES_H
#define UTILITIES_H
#include "polygon.h"
#include <limits>
#include <vector>
#include <set>

namespace geometry {

template<typename Tp>
using Vec = Vector<Tp>;

template<typename Tp>
double Distance(const Polygon<Tp, 2>& poly1, const Polygon<Tp, 2>& poly2) {
    size_t i = 0;
    size_t j = 0;

    double min_dist = std::numeric_limits<double>::max();

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
    assert(min_dist != std::numeric_limits<double>::max());

    return min_dist;
}

template<typename Tp>
std::pair<int, int> FindIntersection(std::vector<Segment<Tp>>& segments) {
    using namespace std;

    struct Event {
        Event(Tp x, int ev, int id)
            : x(x)
            , ev(ev)
            , id(id)
        {}

        bool operator<(const Event& oth) const {
            return (x < oth.x) || (x == oth.x && ev > oth.ev);
        }

        Tp x;
        int ev;
        int id;
    };

    vector<Event> events;
    for (size_t i = 0; i < segments.size(); ++i) {
        Segment<Tp>& s = segments[i];
        s.Reorder();
        events.push_back(Event(s.p1_.x(), 1, i));
        events.push_back(Event(s.p2_.x(), -1, i));
    }
    sort(events.begin(), events.end());

    struct Seg {
        Seg(Segment<Tp>* s, int id)
            : s(s)
            , id(id)
        {}

        double y(const Segment<Tp>& s, Tp x) const {
            Tp x1 = s.p1_.x();
            Tp x2 = s.p2_.x();
            Tp y1 = s.p1_.y();
            Tp y2 = s.p2_.y();

            if (x1 == x2) return y1;
            return y1 + ((y2 - y1) * (x - x1) * 1.0) / (x2 - x1);
        }

        bool operator<(const Seg& seg) const {
            Segment<Tp>& s1 = *s;
            Segment<Tp>& s2 = *seg.s;

            int max_x = max(s1.p1_.x(), s2.p1_.x());
            return y(s1, max_x) < y(s2, max_x);
        }

        Segment<Tp>* s;
        int id;
    };

    set<Seg> s;
    vector<typename set<Seg>::iterator> where(segments.size());

    for (const auto& e: events) {
        if (e.ev == 1) {
            auto& seg = segments[e.id];
            
            auto n = s.lower_bound(Seg(&seg, e.id));
            if (n != s.end() && seg.Intersected(*n->s)) {
                    return {e.id, n->id};
            }

            if (n != s.begin()) {
                auto p = prev(n);
                if (seg.Intersected(*p->s)) {
                    return {e.id, p->id};
                }
            }

            where[e.id] = s.insert(n, Seg(&seg, e.id));
        } else {
            assert(e.ev == -1);
            auto n = s.erase(where[e.id]);
            if (n != s.end() && n != s.begin()) {
                auto p = prev(n);
                if (n->s->Intersected(*p->s)) {
                    return {n->id, p->id};
                }
            }
        }
    }

    return {-1, -1};
}

} // namespace geometry

#endif // UTILITIES_H 
