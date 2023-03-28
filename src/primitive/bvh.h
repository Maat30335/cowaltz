#ifndef BVH_H
#define BVH_H

#include "primitive.h"
#include <memory>
#include <vector>

class BVHnode : public Primitive {
    public:
    static std::shared_ptr<Primitive> createBVH(const std::vector<std::shared_ptr<Primitive>> &prims);
    virtual bool Intersect(const Ray &r, SurfaceInteraction *isect) const;
    virtual Bounds3f WorldBounds() const;
    BVHnode() {};
    BVHnode(std::shared_ptr<Primitive> left, std::shared_ptr<Primitive> right) : left{left}, right{right} {};
    private:
    struct primInfo {
        primInfo() {};
        primInfo(int primNum, Bounds3f bounds) : primNum{primNum}, bounds{bounds}, centroid{0.5 * bounds.pMin + 0.5 * bounds.pMax} {};
        int primNum;
        Bounds3f bounds;
        Point3f centroid;
    };
    static std::shared_ptr<Primitive> recursiveBuild(std::vector<primInfo> &info, const std::vector<std::shared_ptr<Primitive>> &prims, int start, int end, int *totalPrims);
    std::shared_ptr<Primitive> left;
    std::shared_ptr<Primitive> right;
    Bounds3f bounds;
    

    
};




#endif
