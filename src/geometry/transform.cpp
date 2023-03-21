#include "transform.h"

Transform::Transform(const double mat[4][4]){
    m = Matrix4x4(mat[0][0], mat[0][1], mat[0][2], mat[0][3],
                  mat[1][0], mat[1][1], mat[1][2], mat[1][3],
                  mat[2][0], mat[2][1], mat[2][2], mat[2][3],
                  mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
    mInv = Inv(m);
}

bool Transform::operator==(const Transform &t) const{
    return m == t.m;
}

bool Transform::operator!=(const Transform &t) const{
    return m != t.m;
}

Transform Transform::operator*(const Transform &t) const{
    return Transform(m * t.m, t.mInv * mInv);
}



Transform Inv(const Transform &t){
    return Transform(t.mInv, t.m);
}

Transform Translate(const Vector3f &delta){
    Matrix4x4 m(1, 0, 0, delta.x,
                0, 1, 0, delta.y,
                0, 0, 1, delta.z, 
                0, 0, 0,       1);
    Matrix4x4 mInv(1, 0, 0, -delta.x,
                0, 1, 0, -delta.y,
                0, 0, 1, -delta.z, 
                0, 0, 0,       1);
    return Transform(m, mInv);
}
Transform Scale(double s){
    Matrix4x4 m(s, 0, 0, 0,
                0, s, 0, 0,
                0, 0, s, 0,
                0, 0, 0, 1);
    double sInv = 1 / s;
    Matrix4x4 mInv(sInv,   0,   0, 0,
                   0,   sInv,   0, 0,
                   0,     0, sInv, 0,
                   0,     0,   0, 1);
    return Transform(m, mInv);
}

Transform Scale(double x, double y, double z){
    Matrix4x4 m(x, 0, 0, 0,
                0, y, 0, 0,
                0, 0, z, 0,
                0, 0, 0, 1);
    Matrix4x4 mInv(1/x,   0,   0, 0,
                   0,   1/y,   0, 0,
                   0,     0, 1/z, 0,
                   0,     0,   0, 1);
    return Transform(m, mInv);
}
