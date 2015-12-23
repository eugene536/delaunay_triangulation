#ifndef VECTOR_H
#define VECTOR_H
#include <iostream>
#include <vector>
#include <tuple>
#include <functional>
#include <cmath>
#include <cassert>
#include "point.h"

namespace geometry {

template<typename Tp, size_t dim = 2> 
class Vector {
public:
    const size_t sz = dim;

private:
    typedef Vector<Tp, dim> Vec;
    typedef Point<Tp, dim> Pnt;

public: 
    Vector() {
        memset(coordinates_, 0, sizeof(coordinates_));
    }

    template<class...CoordinateType>
    Vector(CoordinateType ... coordinates) {
        static_assert(sizeof...(CoordinateType) == dim, 
                "count of coordinates must be equal to count of dimensions of Vector");
        Init(0, coordinates...);
    }

    Vector(const Pnt& beg, const Pnt& end) {
        for (size_t i = 0; i < dim; ++i) {
            coordinates_[i] = end.coordinates_[i] - beg.coordinates_[i];
        }
    }

    explicit Vector(const Pnt& pnt) {
        *this = pnt;
    }

    Vector(const Vector& oth) {
        *this = oth;
    }

    // sometimes you can't calculate `Length`
    // due to `sqrt` works with not all types (e.g. `mpq_class` doesn't support `sqrt`)
    // use `Length2` in this case, which returns Length ^ 2 and you'll need calculate `sqrt` by hands
    // or you can use `Length2` for perfomance reasons, because `sqrt` is slow =)
    double Length() const {
        Tp sum = 0;
        for (size_t i = 0; i < dim; ++i) {
            sum += coordinates_[i] * coordinates_[i];
        }

        return sqrt(static_cast<double>(sum));
    }

    // calculates `Length` ^ 2
    Tp Length2() const {
        Tp sum = 0;
        for (size_t i = 0; i < dim; ++i) {
            sum += coordinates_[i] * coordinates_[i];
        }

        return sum;
    }

    template<typename T = Tp>
    T DotProduct(const Vec& oth) const {
        T res = 0;
        for (size_t i = 0; i < dim; ++i)
            res += static_cast<T>(coordinates_[i]) * oth.coordinates_[i];

        return res;
    }

    template<typename T = Tp>
    T DotProductSign(const Vec& oth) const {
        return Sign(DotProduct<T>(oth));
    }

    Vector<Tp, 3> CrossProduct(const Vec& oth) const {
        assert(dim >= 2 && dim <= 3); // =(
        Tp* first = coordinates_;
        Tp* second = oth.coordinates_;

        Vector<Tp, 3> result;
        if (dim == 3) {
            result.coordinates_[0] = first[1] * second[2] - first[2] * second[1];
            result.coordinates_[1] = first[2] * second[0] - first[0] * second[2];
        }
        result.coordinates_[2] = first[0] * second[1] - first[1] * second[0];

        return result;
    }

    // returns: -1, 0, 1; for 2D only
    int Rotate(const Vec& oth) const {
        assert(dim == 2);
        return Sign(coordinates_[0] * 1LL * oth.coordinates_[1] - coordinates_[1] * 1LL * oth.coordinates_[0]);
    }

    template<typename T = Tp>
    T Cross(const Vec& oth) const {
        assert(dim == 2);
        return coordinates_[0] * 1LL * oth.coordinates_[1] - coordinates_[1] * 1LL * oth.coordinates_[0];
    }

    Vector<double, dim> Normalize() const {
        Vector<double, dim> res;
        double length = Length();
        for (size_t i = 0; i < dim; ++i)
            res.coordinates_[i] = coordinates_[i] * 1.0 / length;
        return res;
    }

    Vec GetNormal() const {
        assert(dim == 2);
        return Vec(-coordinates_[1], coordinates_[0]);
    }

    Vec& operator=(const Vec& oth) {
        Assign(oth);
        return *this;
    }

    template<typename U>
    Vector<Tp, dim>& operator=(const Point<U, dim>& oth) {
        Assign(oth);
        return *this;
    }

    template<typename U>
    Vector<Tp, dim>& operator=(const Vector<U, dim>& oth) {
        Assign(oth);
        return *this;
    }

    Vec operator-(const Vec& oth) const {
        Vec temp;
        for (size_t i = 0; i < dim; ++i) {
            temp.coordinates_[i] = coordinates_[i] - oth.coordinates_[i];
        }

        return temp;
    }

    Vec operator+(const Vec& oth) const {
        Vec temp;
        for (size_t i = 0; i < dim; ++i) {
            temp.coordinates_[i] = coordinates_[i] + oth.coordinates_[i];
        }

        return temp;
    }

    Vec operator*(const Tp& value) const {
        Vec temp;
        for (size_t i = 0; i < dim; ++i) {
            temp.coordinates_[i] = coordinates_[i] * value;
        }

        return temp;
    }

private:
    template<template <class, size_t> class T, class U>
    void Assign(T<U, dim> oth) {
        for (size_t i = 0; i < dim; ++i) 
            coordinates_[i] = static_cast<U>(oth.coordinates_[i]);
    }

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

    template<typename T>
    inline static int Sign(T val) {
        return (T(0) < val) - (val < T(0));
    }


private:
    Tp coordinates_[dim];

public:
    //template<typename T, size_t d>
    //friend void swap(Vector<T, d>& first, Vector<T, d>& second) {
        //using std::swap;
        //for (size_t i = 0; i < d; ++i) {
            //swap(first.coordinates_[i], second.coordinates_[i]);
        //}
    //}

private:
    template<typename, size_t>
    friend class Vector;

    template<typename, size_t>
    friend class Point;

    template<typename T, size_t d>
    friend std::ostream& operator << (std::ostream& out, const Vector<T, d>& vector);

    template<typename T, size_t d>
    friend std::istream& operator >> (std::istream& in, Vector<T, d>& vector);
};

template<typename Tp, size_t dim = 2>
std::ostream& operator << (std::ostream& out, const Vector<Tp, dim>& vector) {
    for (const Tp& x: vector.coordinates_) {
        out << x << " ";
    }
    return out;
}

template<typename Tp, size_t dim = 2>
std::istream& operator >> (std::istream& in, Vector<Tp, dim>& vector) {
    Tp x;
    for (size_t i = 0; i < dim; ++i) {
        in >> x;
        vector.coordinates_[i] = x;
    }

    return in;
}

} // namespace geometry

#endif // VECTOR_H
