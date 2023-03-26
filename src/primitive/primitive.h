#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <memory>
#include <vector>
#include "interaction.h"
#include "shape.h"
#include "material.h"

class Primitive {
    public:
    virtual bool Intersect(const Ray &r, SurfaceInteraction *isect) const = 0;
    virtual ~Primitive() {};
};

class GeoPrimitive : public Primitive {
    // bounds stuff
    public:
    GeoPrimitive(std::shared_ptr<Shape> shape, std::shared_ptr<Material> material) : shape{shape}, material{material} {};
    virtual bool Intersect(const Ray &r, SurfaceInteraction *isect) const;
    private:
    std::shared_ptr<Shape> shape;
    std::shared_ptr<Material> material;
    // material would also be here
};

class PrimitiveList : public Primitive {
    public:
    virtual bool Intersect(const Ray &r, SurfaceInteraction *isect) const;
    virtual void addPrim(std::shared_ptr<Primitive> primitive);
    virtual void clearPrims();
    private:
    std::vector<std::shared_ptr<Primitive>> primitiveList;
};

#endif
