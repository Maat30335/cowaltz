#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "matrix.h"
#include "vector.h"
#include "ray.h"
#include "normal.h"
#include "interaction.h"

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

    template <typename T> inline Point3<T>
    operator()(const Point3<T> &p) const {
        T x = p.x, y = p.y, z = p.z;
        T xp = m.m[0][0]*x + m.m[0][1]*y + m.m[0][2]*z + m.m[0][3];
        T yp = m.m[1][0]*x + m.m[1][1]*y + m.m[1][2]*z + m.m[1][3];
        T zp = m.m[2][0]*x + m.m[2][1]*y + m.m[2][2]*z + m.m[2][3];
        T wp = m.m[3][0]*x + m.m[3][1]*y + m.m[3][2]*z + m.m[3][3];
        if (wp == 1) return Point3<T>(xp, yp, zp);
        else         return Point3<T>(xp, yp, zp) / wp;
    }

    template <typename T> inline Vector3<T>
    operator()(const Vector3<T> &v) const {
        T x = v.x, y = v.y, z = v.z;
        return Vector3<T>(m.m[0][0]*x + m.m[0][1]*y + m.m[0][2]*z,
                        m.m[1][0]*x + m.m[1][1]*y + m.m[1][2]*z,
                        m.m[2][0]*x + m.m[2][1]*y + m.m[2][2]*z);
    }

    template <typename T> inline Normal3<T>
    operator()(const Normal3<T> &n) const {
        T x = n.x, y = n.y, z = n.z;
        return Normal3<T>(mInv.m[0][0]*x + mInv.m[1][0]*y + mInv.m[2][0]*z,
                        mInv.m[0][1]*x + mInv.m[1][1]*y + mInv.m[2][1]*z,
                        mInv.m[0][2]*x + mInv.m[1][2]*y + mInv.m[2][2]*z);
    }

    inline Ray operator()(const Ray &r) const {
        Point3f o = (*this)(r.o);
        Vector3f d = (*this)(r.d);
        float tMax = r.tMax; // not sure about this???
        return Ray(o, d, tMax);
    }

    inline SurfaceInteraction operator()(const SurfaceInteraction &s) const{
        Point3f p = (*this)(s.p);
        Vector3f wo = (*this)(s.wo);
        Normal3f n = (*this)(s.n);
        return SurfaceInteraction(p, wo, n);
    }
    friend Transform Inv(const Transform &t);

};

Transform Translate(const Vector3f &delta);
Transform Scale(float s);
Transform Scale(float x, float y, float z);





#endif
