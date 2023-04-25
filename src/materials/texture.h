#ifndef TEXTURE_H
#define TEXTURE_H

#include "point.h"
#include "spectrum.h"

// feature: 3d texture?
class Texture {
    public:
    virtual Color getColor(const Point2f &uv) const = 0;
    virtual ~Texture() {};
};

class SolidColor : public Texture {
    public:
    virtual Color getColor(const Point2f &uv) const;
    SolidColor(const Color &color) : color{color} {};

    private:
    Color color;
};






#endif
