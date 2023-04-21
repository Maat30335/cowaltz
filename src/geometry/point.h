#ifndef POINT_H
#define POINT_H

#include "vector.h"
#include "normal.h"
#include <cmath>
#include <cassert>



template <typename T>
class Point3{
    public:
    // public data
    T x, y, z;

    // public methods

    bool HasNaNs() {
        return std::isnan(x) || std::isnan(y) || std::isnan(z);
    }

    Point3() {x = y = z = 0;}

    Point3(T x, T y, T z): x{x}, y{y}, z{z} {
        assert(!HasNaNs());
    }

    template<typename U>
    explicit Point3(const Point3<U> &p): x{(T)p.x} , y{(T)p.y} , z{(T)p.z}{
        assert(!HasNaNs());
    }

    template<typename U>
    explicit operator Vector3<U>() const{
        return Vector3<U>(x, y, z);
    }

    template<typename U>
    explicit operator Normal3<U>() const{
        return Normal3<U>(x, y, z);
    }

    Point3<T> operator+(const Point3<T> &v) const{
        return Point3<T>(x + v.x, y + v.y, z + v.z);
    }

    Point3<T>& operator+=(const Point3<T> &v){
        x =+ v.x;
        y =+ v.y;
        z =+ v.z;
        return *this;
    }

    bool operator==(const Point3<T> &v) const{
        return x == v.x && y == v.y && z == v.z;
    }

    bool operator!=(const Point3<T> &v) const{
        return x != v.x || y != v.y || z != v.z;
    }

    Point3<T> operator*(T s) const{
        return Point3<T>(s * x, s * y, s * z);
    }

    Point3<T>& operator*=(T s){
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }

    Point3<T> operator/(T f) const{
        assert(f != 0);
        double inv = (double) 1 / f;
        return Point3<T>(inv * x, inv * y, inv * z);
    }

    Point3<T>& operator/=(T f){
        assert(f != 0);
        double inv = (double) 1 / f;
        x *= inv;
        y *= inv;
        z *= inv;
        return *this;
    }

    Point3<T> operator-() const{
        return Point3<T>(-x, -y, -z);
    }


    // You can add or subtract a vector to point to offset that point
    Point3<T> operator+(const Vector3<T> &v) const{
        return Point3<T>(x + v.x, y + v.y, z + v.z);
    }

    Point3<T> &operator+=(const Vector3<T> &v){
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Point3<T> operator-(const Vector3<T> &v) const{
        Point3<T>(x - v.x, y - v.y, z - v.z);
    }

    Point3<T> &operator-=(const Vector3<T> &v){
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    Vector3<T> operator-(const Point3<T> &p) const{
        return Vector3<T>(x - p.x, y - p.y, z - p.z);
    }

    T operator[](int i) const { 
           assert(i >= 0 && i <= 2);
           if (i == 0) return x;
           if (i == 1) return y;
           return z;
    }
       
    T &operator[](int i) { 
           assert(i >= 0 && i <= 2);
           if (i == 0) return x;
           if (i == 1) return y;
           return z;
    }
};

template <typename T>
inline std::ostream& operator<<(std::ostream &out, const Point3<T> &v) {
    return out << v.x << ' ' << v.y << ' ' << v.z;
}

template <typename T>
inline Point3<T> operator*(T s,const Point3<T> &v) {
    return v * s;
}

template<typename T>
inline double Distance(const Point3<T> &p1, const Point3<T> &p2){
    return (p1 - p2).Norm();
}

template<typename T>
inline double DistanceSquared(const Point3<T> &p1, const Point3<T> &p2){
    return (p1 - p2).NormSquared();
}

template<typename T>
Point3<T> Lerp(double t, const Point3<T> &p1, const Point3<T> &p2){
    return (1 - t) * p1 + t * p2;
}

typedef Point3<double> Point3f;
typedef Point3<int>   Point3i;


template <typename T>
class Point2{
    public:
    // public data
    T x, y;

    // public methods

    bool HasNaNs() {
        return std::isnan(x) || std::isnan(y);
    }

    Point2() {x = y = 0;}

    Point2(T x, T y): x{x}, y{y} {
        assert(!HasNaNs());
    }

    template<typename U>
    explicit Point2(const Point2<U> &p): x{(T)p.x} , y{(T)p.y}{
        assert(!HasNaNs());
    }

    Point2<T> operator+(const Point2<T> &v) const{
        return Point2(x + v.x, y + v.y);
    }

    Point2<T>& operator+=(const Point2<T> &v){
        x =+ v.x;
        y =+ v.y;
        return *this;
    }

    bool operator==(const Point2<T> &v) const{
        return x == v.x && y == v.y;;
    }

    bool operator!=(const Point2<T> &v) const{
        return x != v.x || y != v.y;
    }

    Point2<T> operator*(T s) const{
        return Point2(s * x, s * y);
    }

    Point2<T>& operator*=(T s){
        x *= s;
        y *= s;
        return *this;
    }

    Point2<T> operator/(T f) const{
        assert(f != 0);
        double inv = (double) 1 / f;
        return Point2(inv * x, inv * y);
    }

    Point2<T>& operator/=(T f){
        assert(f != 0);
        double inv = (double) 1 / f;
        x *= inv;
        y *= inv;
        return *this;
    }

    Point2<T> operator-() const{
        return Point2(-x, -y);
    }

    Vector2<T> operator-(const Point2<T> &p) const{
        return Vector2<T>(x - p.x, y - p.y);
    }
};

template <typename T>
inline std::ostream& operator<<(std::ostream &out, const Point2<T> &v) {
    return out << v.x << ' ' << v.y;
}

template <typename T>
inline Point2<T> operator*(T s,const Point2<T> &p) {
    return p * s;
}

typedef Point2<double> Point2f;
typedef Point2<int>   Point2i;


#endif
