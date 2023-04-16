#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <memory>
#include <vector>
#include "shape.h"
#include "material.h"

struct SurfaceInteraction;

class Primitive {
    public:
    virtual bool Intersect(const Ray &r, SurfaceInteraction *isect) const = 0;
    virtual bool IntersectP(const Ray &r) const;
    virtual Bounds3f WorldBounds() const = 0;
    virtual ~Primitive() {};
};

class GeoPrimitive : public Primitive {
    // bounds stuff
    public:
    GeoPrimitive(std::shared_ptr<Shape> shape, std::shared_ptr<Material> material) : shape{shape}, material{material} {};
    virtual bool Intersect(const Ray &r, SurfaceInteraction *isect) const;
    virtual Bounds3f WorldBounds() const;
    private:
    std::shared_ptr<Shape> shape;
    std::shared_ptr<Material> material;
};

class PrimitiveList : public Primitive {
    public:
    virtual bool Intersect(const Ray &r, SurfaceInteraction *isect) const;
    virtual bool IntersectP(const Ray &r) const;
    virtual void addPrim(std::shared_ptr<Primitive> primitive);
    virtual void clearPrims();
    virtual Bounds3f WorldBounds() const;
    private:
    Bounds3f bounds;
    std::vector<std::shared_ptr<Primitive>> primitiveList;
};

#endif
