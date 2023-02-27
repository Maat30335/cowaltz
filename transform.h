#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "matrix.h"
#include "vector.h"

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
    friend Transform Inv(const Transform &t);

};

Transform Translate(const Vector3f &delta);
Transform Scale(float s);
Transform Scale(float x, float y, float z);



#endif
