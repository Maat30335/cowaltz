#include "transform.h"
#include "useful.h"

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

Transform RotateX(double theta) {
    double sinTheta = std::sin(Radians(theta));
    double cosTheta = std::cos(Radians(theta));
    Matrix4x4 m(1,        0,         0, 0, 
                0, cosTheta, -sinTheta, 0,
                0, sinTheta,  cosTheta, 0,
                0,        0,         0, 1);
    return Transform(m, Transpose(m));
}

Transform RotateY(double theta) {
    double sinTheta = std::sin(Radians(theta));
    double cosTheta = std::cos(Radians(theta));
    Matrix4x4 m(cosTheta,           0,         sinTheta, 0, 
                        0,         1, 0, 0,
                -sinTheta,  0,  cosTheta, 0,
                        0,        0,         0, 1);
    return Transform(m, Transpose(m));
}

Transform RotateZ(double theta) {
    double sinTheta = std::sin(Radians(theta));
    double cosTheta = std::cos(Radians(theta));
    Matrix4x4 m(cosTheta, -sinTheta,    0, 0, 
                sinTheta,  cosTheta, 0, 0,
                0, 0,1, 0,
                0,        0,         0, 1);
    return Transform(m, Transpose(m));
}

Transform ToSpace(const Vector3f &v){
    Vector3f t1, t2;
    CoordinateSystem(v, &t1, &t2);
    Matrix4x4 mInv(t1.x, v.x, t2.x, 0,
                t1.y, v.y, t2.y, 0,
                t1.z, v.z, t2.z, 0,
                0, 0, 0, 1);
    Matrix4x4 m(t1.x, t1.y, t1.z, 0,
                v.x, v.y, v.z, 0,
                t2.x, t2.y, t2.z, 0,
                0, 0, 0, 1);
    return Transform(m, mInv);
}

Transform ToSpace(const Vector3f &v0, const Vector3f &v1, const Vector3f &v2){
    Matrix4x4 mInv(v0.x, v1.x, v2.x, 0,
                v0.y, v1.y, v2.y, 0,
                v0.z, v1.z, v2.z, 0,
                0, 0, 0, 1);
    Matrix4x4 m = Inv(mInv);
    return Transform(m, mInv);
}

Transform LookAt(const Point3f &pos, const Point3f &look, const Vector3f &up) {
    Matrix4x4 cameraToWorld;
    cameraToWorld.m[0][3] = pos.x;
    cameraToWorld.m[1][3] = pos.y;
    cameraToWorld.m[2][3] = pos.z;
    cameraToWorld.m[3][3] = 1;

    Vector3f dir = Normalize(pos - look);
        //ah yes the left handed system
       Vector3f right = Normalize(Cross(Normalize(up), dir));
       Vector3f newUp = Cross(dir, right);
       cameraToWorld.m[0][0] = right.x;
       cameraToWorld.m[1][0] = right.y;
       cameraToWorld.m[2][0] = right.z;
       cameraToWorld.m[3][0] = 0.;
       cameraToWorld.m[0][1] = newUp.x;
       cameraToWorld.m[1][1] = newUp.y;
       cameraToWorld.m[2][1] = newUp.z;
       cameraToWorld.m[3][1] = 0.;
       cameraToWorld.m[0][2] = dir.x;
       cameraToWorld.m[1][2] = dir.y;
       cameraToWorld.m[2][2] = dir.z;
       cameraToWorld.m[3][2] = 0.;

       return Transform(cameraToWorld, Inv(cameraToWorld));
}
