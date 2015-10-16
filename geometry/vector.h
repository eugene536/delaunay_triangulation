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
private:
    typedef Vector<Tp, dim> Vec;
    typedef Point<Tp, dim> Pnt;

public: 
    Vector() {
        coordinates_.resize(dim);
    }

    template<class...CoordinateType>
    Vector(CoordinateType ... coordinates) {
        static_assert(sizeof...(CoordinateType) == dim, 
                "count of coordinates must be equal to count of dimensions of Vector");
        coordinates_ = {static_cast<Tp>(coordinates)...};
    }

    Vector(const Pnt& beg, const Pnt& end) {
        for (size_t i = 0; i < dim; ++i) {
            coordinates_.push_back(end.coordinates_[i] - beg.coordinates_[i]);
        }
    }

    explicit Vector(const Pnt& pnt) {
        coordinates_ = pnt.coordinates_;
    }

    double Length() const {
        Tp sum = 0;
        for (size_t i = 0; i < dim; ++i) {
            sum += coordinates_[i] * coordinates_[i];
        }

        return sqrt(static_cast<double>(sum));
    }

    Tp DotProduct(const Vec& oth) const {
        Tp res = 0;
        for (size_t i = 0; i < dim; ++i) {
            res += coordinates_[i] * oth.coordinates_[i];
        }

        return res;
    }

    Vector<Tp, 3> CrossProduct(const Vec& oth) const {
        assert(dim < 4); // =(
        std::vector<Tp> first = coordinates_;
        first.resize(3);

        std::vector<Tp> second = oth.coordinates_;
        second.resize(3);

        Vector<Tp, 3> result;
        result.coordinates_[0] = first[1] * second[2] - first[2] * second[1];
        result.coordinates_[1] = first[2] * second[0] - first[0] * second[2];
        result.coordinates_[2] = first[0] * second[1] - first[1] * second[0];

        return result;
    }

    Vec operator-(const Vec& oth) const {
        Vec temp;
        for (size_t i = 0; i < coordinates_.size(); ++i) {
            temp.coordinates_.push_back(coordinates_[i] - oth.coordinates_[i]);
        }

        return temp;
    }

    Vec operator+(const Vec& oth) const {
        Vec temp;
        for (size_t i = 0; i < coordinates_.size(); ++i) {
            temp.coordinates_.push_back(coordinates_[i] + oth.coordinates_[i]);
        }

        return temp;
    }

    Vec operator*(const Tp& value) const {
        Vec temp;
        for (size_t i = 0; i < coordinates_.size(); ++i) {
            temp.coordinates_.push_back(coordinates_[i] * value);
        }

        return temp;
    }

private:
    std::vector<Tp> coordinates_;

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
    if (vector.coordinates_.size() > 0) {
        for (size_t i = 0; i < vector.coordinates_.size() - 1; ++i) {
            out << vector.coordinates_[i] << ", ";
        }
        out << vector.coordinates_.back();
    }
    return out << ")";
}

template<typename Tp, size_t dim = 2>
std::istream& operator >> (std::istream& in, Vector<Tp, dim>& vector) {
    vector.coordinates_.clear();
    Tp x;
    for (size_t i = 0; i < dim; ++i) {
        in >> x;
        vector.coordinates_.push_back(x);
    }

    return in;
}

#endif // VECTOR_H
