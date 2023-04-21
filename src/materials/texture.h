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

class Image_Texture : public Texture {
    public:
    const static int bytes_per_pixel = 3;
    virtual Color getColor(const Point2f &uv) const;
    Image_Texture() : data(nullptr), width(0), height(0), bytes_per_scanline(0) {};
    Image_Texture(const char*  filename);
    ~Image_Texture();
    private:
        unsigned char *data;
        int width, height;
        int bytes_per_scanline;

};






#endif
