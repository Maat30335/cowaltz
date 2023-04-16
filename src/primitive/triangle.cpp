#include "triangle.h"
#include "transform.h"
#include "interaction.h"
#include "useful.h"
#include <iostream>
#include <cmath>
#include <iostream>

TriangleMesh::TriangleMesh(const Transform &ObjectToWorld,
        int nTriangles, const int *vertexIndices, int nVertices,
        const Point3f *P, const Normal3f *N,
        const Point2f *UV) : nTriangles(nTriangles), nVertices(nVertices), 
      vertexIndices(vertexIndices, vertexIndices + 3 * nTriangles) {
        p.reset(new Point3f[nVertices]);
       for (int i = 0; i < nVertices; ++i)
           p[i] = ObjectToWorld(P[i]);

        if (UV) {
           uv.reset(new Point2f[nVertices]);
           memcpy(uv.get(), UV, nVertices * sizeof(Point2f));
       }
        if (N) {
           n.reset(new Normal3f[nVertices]);
           for (int i = 0; i < nVertices; ++i)
               n[i] = ObjectToWorld(N[i]);
       }
}

std::vector<std::shared_ptr<Shape>> CreateTriangleMesh(
        const Transform *ObjectToWorld, const Transform *WorldToObject,
        int nTriangles, const int *vertexIndices, int nVertices, const Point3f *p,
        const Normal3f *n, const Point2f *uv) {
    std::shared_ptr<TriangleMesh> mesh = std::make_shared<TriangleMesh>(
        *ObjectToWorld, nTriangles, vertexIndices, nVertices, p, n, uv);
    std::vector<std::shared_ptr<Shape>> tris;
    for (int i = 0; i < nTriangles; ++i){
        tris.push_back(std::make_shared<Triangle>(ObjectToWorld,
            WorldToObject, mesh, i));
    }
    return tris;
}

Triangle::Triangle(const Transform *ObjectToWorld, const Transform *WorldToObject,
                    const std::shared_ptr<TriangleMesh> &mesh, int meshNum) : 
                    Shape(ObjectToWorld, WorldToObject), mesh(mesh){
                        nv = &mesh->vertexIndices[3 * meshNum];
                    }

Bounds3f Triangle::ObjectBounds() const{
    return (*WorldToObject)(Union(Bounds3f(mesh->p[nv[0]], mesh->p[nv[1]]), mesh->p[nv[2]]));
}

Bounds3f Triangle::WorldBounds() const{
    return Union(Bounds3f(mesh->p[nv[0]], mesh->p[nv[1]]), mesh->p[nv[2]]);
}

void Triangle::GetUVs(Point2f uv[3]) const {
           if (mesh->uv) {
               uv[0] = mesh->uv[nv[0]];
               uv[1] = mesh->uv[nv[1]];
               uv[2] = mesh->uv[nv[2]];
           } else {
               uv[0] = Point2f(0, 0);
               uv[1] = Point2f(1, 0);
               uv[2] = Point2f(1, 1);
           }
}

bool Triangle::Intersect(const Ray &r, double *tHit, SurfaceInteraction *isect) const{
    const Point3f &p0 = mesh->p[nv[0]];
    const Point3f &p1 = mesh->p[nv[1]];
    const Point3f &p2 = mesh->p[nv[2]];

    Vector3f v0v1 = p1 - p0;
    Vector3f v0v2 = p2 - p0;
    Vector3f pvec = Cross(r.d, v0v2);
    double det = Dot(v0v1, pvec);

    if(std::abs(det) < Epsilon) return false;

    double invDet = 1 / det;
    Vector3f tvec = r.o - p0;
    double u = Dot(tvec, pvec) * invDet;
    if (u < 0 || u > 1) return false;

    Vector3f qvec = Cross(tvec, v0v1);
    double v = Dot(r.d, qvec) * invDet;
    if (v < 0 || u + v > 1) return false;

    double tTemp = Dot(v0v2, qvec) * invDet;
    if(tTemp < 0 || tTemp > r.tMax) return false;

    *tHit = tTemp;

    // the bad but hopefully working code



    Point3f pHit = r(*tHit);
    double w = 1 - u - v;
    Vector3f dpdu;
    Vector3f dpdv;
    Point2f uv[3];
    GetUVs(uv);

    Point2f uvHit = w * uv[0] + u * uv[1] + v * uv[2];

    Vector2f duv02 = uv[0] - uv[2], duv12 = uv[1] - uv[2];
    Vector3f dp02 = -v0v2, dp12 = p1 - p2;

    det = duv02[0] * duv12[1] - duv02[1] * duv12[0];

    if(det == 0){
        CoordinateSystem(Cross(v0v1, v0v2), &dpdu, &dpdv);
    }else{
        invDet = 1 / det;
        dpdu = ( duv12[1] * dp02 - duv02[1] * dp12) * invDet;
        dpdv = (-duv12[0] * dp02 + duv02[0] * dp12) * invDet;
    }

    Normal3f nHit;

    if(mesh->n){
        nHit = Normalize(w * mesh->n[nv[0]] + u * mesh->n[nv[1]] + v * mesh->n[nv[2]]);
        dpdu = Normalize(dpdu - (Dot(dpdu, nHit)) * (Vector3f)nHit);
        dpdv = Normalize(dpdv - (Dot(dpdv, nHit)) * (Vector3f)nHit);

    }else{

        nHit = (Normal3f)Normalize((Cross(p0 - p2, p1 - p2)));
        // if we don't have normals, dont assume we have good partial derivatives (for now)
        CoordinateSystem((Vector3f)nHit, &dpdu, &dpdv);

    }

    *isect = SurfaceInteraction(pHit, uvHit, dpdu, dpdv, -r.d, nHit);

    return true;


}


