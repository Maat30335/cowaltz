#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>
#include <cassert>
#include <iostream>

template <typename T>
class Normal3;

template <typename T> 
class Vector3{
    public:
    // public data
    T x, y, z;

    // public methods
    explicit Vector3(const Normal3<T> &n);

    bool HasNaNs() {
        return std::isnan(x) || std::isnan(y) || std::isnan(z);
    }

    Vector3() {x = y = z = 0;}

    Vector3(T x, T y, T z): x{x}, y{y}, z{z} {
        assert(!HasNaNs());
    }

    T operator[](int i) const{
        assert(0 <= i && i <= 2);
        if(i == 0) return x;
        if (i == 1) return y;
        return z;
    }

    T &operator[](int i) {
        assert(0 <= i && i <= 2);
        if(i == 0) return x;
        if (i == 1) return y;
        return z;
    }

    Vector3<T> operator+(const Vector3<T> &v) const{
        return Vector3(x + v.x, y + v.y, z + v.z);
    }

    Vector3<T>& operator+=(const Vector3<T> &v){
        x =+ v.x;
        y =+ v.y;
        z =+ v.z;
        return *this;
    }

    Vector3<T> operator-(const Vector3<T> &v) const{
        return Vector3(x - v.x, y - v.y, z - v.z);
    }
        
    Vector3<T>& operator-=(const Vector3<T> &v){
        x =- v.x;
        y =- v.y;
        z =- v.z;
        return *this;
    }

    bool operator==(const Vector3<T> &v) const{
        return x == v.x && y == v.y && z == v.z;
    }

    bool operator!=(const Vector3<T> &v) const{
        return x != v.x || y != v.y || z != v.z;
    }

    Vector3<T> operator*(T s) const{
        return Vector3(s * x, s * y, s * z);
    }

    Vector3<T>& operator*=(T s){
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }

    Vector3<T> operator/(T f) const{
        assert(f != 0);
        double inv = (double) 1 / f;
        return Vector3(inv * x, inv * y, inv * z);
    }

    Vector3<T>& operator/=(T f){
        assert(f != 0);
        double inv = (double) 1 / f;
        x *= inv;
        y *= inv;
        z *= inv;
        return *this;
    }

    Vector3<T> operator-() const{
        return Vector3(-x, -y, -z);
    }

    double NormSquared() const{
        return x * x + y * y + z * z;
    }

    double Norm() const{
        return std::sqrt(NormSquared());
    }

};

template <typename T>
inline std::ostream& operator<<(std::ostream &out, const Vector3<T> &v) {
    return out << v.x << ' ' << v.y << ' ' << v.z;
}



template <typename T>
inline Vector3<T> operator*(T s,const Vector3<T> &v) {
    return v * s;
}

template <typename T>
Vector3<T> Abs(const Vector3<T> &v){
    return Vector3<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z));
}

template <typename T>
inline T Dot(const Vector3<T> &v1, const Vector3<T> &v2){
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

template <typename T>
inline T AbsDot(const Vector3<T> &v1, const Vector3<T> &v2){
    return std::abs(Dot(v1, v2));
}

template <typename T>
inline Vector3<T> Cross(const Vector3<T> &v1, const Vector3<T> &v2){
    return Vector3<T>(v1.y * v2.z - v1.z * v2.y,
                v1.z * v2.x - v1.x * v2.z,
                v1.x * v2.y - v1.y * v2.x);
}

template <typename T>
inline Vector3<T> Normalize(const Vector3<T> &v){
    return v / v.Norm();
}

template <typename T> inline void
CoordinateSystem(const Vector3<T> &v1, Vector3<T> *v2, Vector3<T> *v3) {
    if (std::abs(v1.x) > std::abs(v1.y))
        *v2 = Vector3<T>(-v1.z, 0, v1.x) /
              std::sqrt(v1.x * v1.x + v1.z * v1.z);
    else
        *v2 = Vector3<T>(0, v1.z, -v1.y) /
              std::sqrt(v1.y * v1.y + v1.z * v1.z);
    *v3 = Cross(v1, *v2);
}

typedef Vector3<double> Vector3f;
typedef Vector3<int> Vector3i;


template <typename T> 
class Vector2{
    public:
    // public data
    T x, y;

    bool HasNaNs() {
        return std::isnan(x) || std::isnan(y);
    }

    Vector2() {x = y = 0;}

    Vector2(T x, T y): x{x}, y{y} {
        assert(!HasNaNs());
    }

    T operator[](int i) const{
        assert(0 <= i && i <= 1);
        if(i == 0) return x;
        return y;
    }

    T &operator[](int i) {
        assert(0 <= i && i <= 2);
        if(i == 0) return x;
        return y;
    }

    Vector2<T> operator+(const Vector2<T> &v) const{
        return Vector2(x + v.x, y + v.y);
    }

    Vector2<T>& operator+=(const Vector2<T> &v){
        x =+ v.x;
        y =+ v.y;
        return *this;
    }

    Vector2<T> operator-(const Vector2<T> &v) const{
        return Vector2(x - v.x, y - v.y);
    }
        
    Vector2<T>& operator-=(const Vector2<T> &v){
        x =- v.x;
        y =- v.y;
        return *this;
    }

    bool operator==(const Vector2<T> &v) const{
        return x == v.x && y == v.y;
    }

    bool operator!=(const Vector2<T> &v) const{
        return x != v.x || y != v.y;
    }

    Vector2<T> operator*(T s) const{
        return Vector2(s * x, s * y);
    }

    Vector2<T>& operator*=(T s){
        x *= s;
        y *= s;
        return *this;
    }

    Vector2<T> operator/(T f) const{
        assert(f != 0);
        double inv = (double) 1 / f;
        return Vector2(inv * x, inv * y);
    }

    Vector2<T>& operator/=(T f){
        assert(f != 0);
        double inv = (double) 1 / f;
        x *= inv;
        y *= inv;
        return *this;
    }

    Vector2<T> operator-() const{
        return Vector2(-x, -y);
    }

    double NormSquared() const{
        return x * x + y * y;
    }

    double Norm() const{
        return std::sqrt(NormSquared());
    }

};

template <typename T>
inline Vector2<T> operator*(T s,const Vector2<T> &v) {
    return v * s;
}

typedef Vector2<double> Vector2f;
typedef Vector2<int> Vector2i;

#endif


