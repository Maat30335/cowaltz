#ifndef NORMAL_H
#define NORMAL_H

#include <cmath>
#include <cassert>
#include "vector.h"


template <typename T> 
class Normal3{
    public:
    // public data
    T x, y, z;

    // public methods

    bool HasNaNs() const{
        return std::isnan(x) || std::isnan(y) || std::isnan(z);
    }

    Normal3() {x = y = z = 0;}

    Normal3(T x, T y, T z): x{x}, y{y}, z{z} {
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

    Normal3<T> operator+(const Normal3<T> &v) const{
        return Normal3(x + v.x, y + v.y, z + v.z);
    }

    Normal3<T>& operator+=(const Normal3<T> &v){
        x =+ v.x;
        y =+ v.y;
        z =+ v.z;
        return *this;
    }

    Normal3<T> operator-(const Normal3<T> &v) const{
        return Normal3(x - v.x, y - v.y, z - v.z);
    }
        
    Normal3<T>& operator-=(const Normal3<T> &v){
        x =- v.x;
        y =- v.y;
        z =- v.z;
        return *this;
    }

    bool operator==(const Normal3<T> &v) const{
        return x == v.x && y == v.y && z == v.z;
    }

    bool operator!=(const Normal3<T> &v) const{
        return x != v.x || y != v.y || z != v.z;
    }

    Normal3<T> operator*(T s) const{
        return Normal3(s * x, s * y, s * z);
    }

    Normal3<T>& operator*=(T s){
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }

    Normal3<T> operator/(T f) const{
        assert(f != 0);
        float inv = (float) 1 / f;
        return Normal3(inv * x, inv * y, inv * z);
    }

    Normal3<T>& operator/=(T f){
        assert(f != 0);
        float inv = (float) 1 / f;
        x *= inv;
        y *= inv;
        z *= inv;
        return *this;
    }

    Normal3<T> operator-() const{
        return Normal3(-x, -y, -z);
    }

    float NormSquared() const{
        return x * x + y * y + z * z;
    }

    float Norm() const{
        return std::sqrt(NormSquared());
    }


};



template <typename T>
inline Normal3<T> operator*(T s,const Normal3<T> &v) {
    return v * s;
}

template <typename T>
Normal3<T> Abs(const Normal3<T> &v){
    return Normal3<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z));
}

template <typename T>
inline T Dot(const Normal3<T> &v1, const Normal3<T> &v2){
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

template <typename T>
inline T Dot(const Normal3<T> &v1, const Vector3<T> &v2){
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

template <typename T>
inline T Dot(const Vector3<T> &v1, const Normal3<T> &v2){
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}


template <typename T>
inline T AbsDot(const Normal3<T> &v1, const Normal3<T> &v2){
    return std::abs(Dot(v1, v2));
}

template <typename T>
inline T AbsDot(const Normal3<T> &v1, const Vector3<T> &v2){
    return std::abs(Dot(v1, v2));
}

template <typename T>
inline T AbsDot(const Vector3<T> &v1, const Normal3<T> &v2){
    return std::abs(Dot(v1, v2));
}

template <typename T>
inline Normal3<T> Cross(const Normal3<T> &v1, const Normal3<T> &v2){
    return Normal3<T>(v1.y * v2.z - v1.z * v2.y,
                v1.z * v2.x - v1.x * v2.z,
                v1.x * v2.y - v1.y * v2.x);
}

template <typename T>
inline Normal3<T> Normalize(const Normal3<T> &v){
    return v / v.Norm();
}

template<typename T>
inline Normal3<T> FaceForward(const Normal3<T> &n, const Vector3<T> &v){
    return (Dot(n, v) < 0.f) ? -n : n;
}

template <typename T> 
inline Vector3<T>::Vector3(const Normal3<T> &n) : x(n.x), y(n.y), z(n.z) {
    assert(!n.HasNaNs());
}

typedef Normal3<float> Normal3f;
typedef Normal3<int> Normal3i;

#endif
