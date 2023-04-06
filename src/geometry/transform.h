#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "matrix.h"
#include "vector.h"
#include "ray.h"
#include "normal.h"
#include "bounds.h"
#include "interaction.h"

class Transform{
private:
    Matrix4x4 m, mInv;
public:
    Transform() {};
    Transform(const double mat[4][4]);
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
        return Normalize(Normal3<T>(mInv.m[0][0]*x + mInv.m[1][0]*y + mInv.m[2][0]*z,
                        mInv.m[0][1]*x + mInv.m[1][1]*y + mInv.m[2][1]*z,
                        mInv.m[0][2]*x + mInv.m[1][2]*y + mInv.m[2][2]*z));
    }

    template <typename T> inline Bounds3<T>
    operator()(const Bounds3<T> &A) const{
        Bounds3f B = Bounds3f(Point3f(m.m[0][3], m.m[1][3], m.m[2][3]));
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                double a = m.m[i][j] * A.pMin[j];
                double b = m.m[i][j] * A.pMax[j];
                B.pMin[i] += a < b ? a : b;
                B.pMax[i] += a < b ? b : a;
            }
        }
        return B;
    }

    inline Ray operator()(const Ray &r) const {
        Point3f o = (*this)(r.o);
        Vector3f d = (*this)(r.d);
        double tMax = r.tMax; // not sure about this???
        return Ray(o, d, tMax);
    }

    inline SurfaceInteraction operator()(const SurfaceInteraction &s) const{
        Point2f uv = s.uv;
        Vector3f dpdu = (*this)(s.dpdu);
        Vector3f dpdv = (*this)(s.dpdv);
        Point3f p = (*this)(s.p);
        Vector3f wo = (*this)(s.wo);
        Normal3f n = (*this)(s.n);
        return SurfaceInteraction(p, uv, dpdu, dpdv, wo, n);
    }
    friend Transform Inv(const Transform &t);

};

Transform Translate(const Vector3f &delta);
Transform Scale(double s);
Transform Scale(double x, double y, double z);
Transform RotateX(double theta);
Transform RotateY(double theta);
Transform RotateZ(double theta);
Transform ToSpace(const Vector3f &v);
Transform LookAt(const Point3f &pos, const Point3f &look, const Vector3f &up);




#endif
