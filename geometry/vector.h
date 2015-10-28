#ifndef VECTOR_H
#define VECTOR_H
#include <iostream>
#include <vector>
#include <tuple>
#include <functional>
#include <cmath>
#include <cassert>
#include "point.h"

template<typename Tp, size_t dim = 2> 
class Vector {
public:
    const size_t sz = dim;

private:
    typedef Vector<Tp, dim> Vec;
    typedef Point<Tp, dim> Pnt;

public: 
    Vector()
    {
        memset(coordinates_, 0, sizeof(coordinates_));
    }

    template<class...CoordinateType>
    Vector(CoordinateType ... coordinates) 
    {
        static_assert(sizeof...(CoordinateType) == dim, 
                "count of coordinates must be equal to count of dimensions of Vector");
        init(0, coordinates...);
    }

    template<class FirstCoordinateType, class...CoordinateType>
    void init(int lev, FirstCoordinateType first, CoordinateType...coordinates) {
        static_assert(std::is_same<FirstCoordinateType, Tp>::value,
                "types must be equal to Tp");

        coordinates_[lev] = first;
        init(lev + 1, coordinates...);
    }

    void init(int lev) {
        assert(lev == dim);
    }

    Vector(const Pnt& beg, const Pnt& end) {
        for (size_t i = 0; i < dim; ++i) {
            coordinates_[i] = end.coordinates_[i] - beg.coordinates_[i];
        }
    }

    explicit Vector(const Pnt& pnt) {
        coordinates_ = pnt.coordinates_;
    }

    Vector(const Vector& oth) 
    {
        *this = oth;
    }

    Vector(Vector&& oth)
    {
        *this = std::move(oth);
    }

    double Length() const {
        Tp sum = 0;
        for (size_t i = 0; i < dim; ++i) {
            sum += coordinates_[i] * coordinates_[i];
        }

        return sqrt(static_cast<double>(sum));
    }

    Tp Length2() const {
        Tp sum = 0;
        for (size_t i = 0; i < dim; ++i) {
            sum += coordinates_[i] * coordinates_[i];
        }

        return sum;
    }

    Tp DotProduct(const Vec& oth) const {
        Tp res = 0;
        for (size_t i = 0; i < dim; ++i) {
            res += coordinates_[i] * oth.coordinates_[i];
        }

        return res;
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

    template<typename T>
    inline static int sgn(T val) {
        return (T(0) < val) - (val < T(0));
    }

    int Rotate(const Vec& oth) const {
        assert(dim == 2);
        return sgn(coordinates_[0] * oth.coordinates_[1] - coordinates_[1] * oth.coordinates_[0]);
    }

    Vec& operator=(Vec&& oth) {
        assert(this != &oth);
        coordinates_ = oth.coordinates_;

        return *this;
    }

    Vec& operator=(const Pnt& oth) {
        std::copy(oth.coordinates_, oth.coordinates_ + sz, coordinates_);
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
    Tp coordinates_[dim];

private:
    template<typename T, size_t d>
    friend class Vector;

    template<typename T, size_t d>
    friend std::ostream& operator << (std::ostream& out, const Vector<T, d>& vector);

    template<typename T, size_t d>
    friend std::istream& operator >> (std::istream& in, Vector<T, d>& vector);
};

template<typename Tp, size_t dim = 2>
std::ostream& operator << (std::ostream& out, const Vector<Tp, dim>& vector) {
    out << "v(";
    if (vector.sz > 0) {
        for (size_t i = 0; i < vector.sz - 1; ++i) {
            out << vector.coordinates_[i] << ", ";
        }
        out << vector.coordinates_[vector.sz - 1];
    }
    return out << ")";
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

#endif // VECTOR_H
