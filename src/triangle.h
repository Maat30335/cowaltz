#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <memory>
#include "shape.h"
#include "point.h"
#include "normal.h"
#include "bounds.h"
#include <vector>

struct TriangleMesh {
    TriangleMesh(const Transform &ObjectToWorld,
        int nTriangles, const int *vertexIndices, int nVertices,
        const Point3f *P, const Normal3f *N,
        const Point2f *UV);
    const int nTriangles, nVertices;
    std::vector<int> vertexIndices;
    std::unique_ptr<Point3f[]> p;
    std::unique_ptr<Normal3f[]> n;
    std::unique_ptr<Point2f[]> uv;

};

class Triangle : public Shape {
    public:
    Triangle(const Transform *ObjectToWorld, const Transform *WorldToObject,
             const std::shared_ptr<TriangleMesh> &mesh, int meshNum);
    virtual bool Intersect(const Ray &r, double *tHit, SurfaceInteraction *isect) const;
    virtual Bounds3f ObjectBounds() const;
    virtual Bounds3f WorldBounds() const;
    private:
    void GetUVs(Point2f uv[3]) const;
    std::shared_ptr<TriangleMesh> mesh;
    const int *nv;
};

std::vector<std::shared_ptr<Shape>> CreateTriangleMesh(
        const Transform *ObjectToWorld, const Transform *WorldToObject,
        int nTriangles, const int *vertexIndices, int nVertices, const Point3f *p,
        const Normal3f *n, const Point2f *uv);


#endif
