#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "matrix.h"
#include "vector.h"
#include "ray.h"
#include "normal.h"

class Transform{
private:
    Matrix4x4 m, mInv;
public:
    Transform() {};
    Transform(const float mat[4][4]);
    Transform(const Matrix4x4 &m) : m{m}, mInv{Inv(m)} {};
    Transform(const Matrix4x4 &m, const Matrix4x4 &mInv) : m{m}, mInv{mInv} {};
    bool operator==(const Transform &t) const;
    bool operator!=(const Transform &t) const;
    Transform operator*(const Transform &t) const;

    template <typename T>
    inline Point3<T> operator()(const Point3<T> &p) const;

    template <typename T> 
    inline Vector3<T> operator()(const Vector3<T> &v) const;

    template <typename T> 
    inline Normal3<T> operator()(const Normal3<T> &n) const;

    inline Ray operator()(const Ray &r) const;

    friend Transform Inv(const Transform &t);

};

Transform Translate(const Vector3f &delta);
Transform Scale(float s);
Transform Scale(float x, float y, float z);





#endif
