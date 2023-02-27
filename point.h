#ifndef POINT_H
#define POINT_H

#include "vector.h"


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
        Assert(!HasNaNs());
    }

    template<typename U>
    explicit Point3(const Point3<U> &p): x{(T)p.x} , y{(T)p.y} , z{(T)p.z}{
        Assert(!HasNaNs());
    }

    template<U>
    explicit operator Vector3<U>() const{
        return Vector<U>(x, y, z);
    }

    Point3<T> operator+(const Point3<T> &v) const{
        return Point3(x + v.x, y + v.y, z + v.z);
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
        return Point3(s * x, s * y, s * z);
    }

    Point3<T>& operator*=(T s){
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }

    Point3<T> operator/(T f) const{
        Assert(f != 0);
        float inv = (float) 1 / f;
        return Point3(inv * x, inv * y, inv * z);
    }

    Point3<T>& operator/=(T f){
        Assert(f != 0);
        float inv = (float) 1 / f;
        x *= inv;
        y *= inv;
        z *= inv;
        return *this;
    }

    Point3<T> operator-() const{
        return Point3(-x, -y, -z);
    }


    // You can add or subtract a vector to point to offset that point
    Point3<T> operator+(const Vector3<T> &v) const{
        Point3(x + v.x, y + v.y, z + v.z);
    }

    Point3<T> &operator+=(const Vector3<T> &v){
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Point3<T> operator-(const Vector3<T> &v) const{
        Point3<T>(x - p.x, y - p.y, z - p.z)
    }

    Point3<T> &operator-=(const Vector3<T> &v){
        x -= p.x;
        y -= p.y;
        z -= p.z;
        return *this;
    }

    Vector3<T> operator-(const Point3<T> &p) const{
        return Vector3<T>(x - p.x, y - p.y, z - p.z);
    }
};

template<typename T>
inline float Distance(const Point3<T> &p1, const Point3<T> &p2){
    return (p1 - p2).Norm();
}

template<typename T>
inline float DistanceSquared(const Point3<T> &p1, const Point3<T> &p2){
    return (p1 - p2).NormSquared();
}

template<typename T>
Point3<T> Lerp(float t, const Point3<T> &p1, const Point3<T> &p2){
    return (1 - t) * p1 + t * p2;
}

typedef Point3<float> Point3f;
typedef Point3<int>   Point3i;

#endif
