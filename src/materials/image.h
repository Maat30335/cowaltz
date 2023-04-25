#ifndef IMAGE_H
#define IMAGE_H

#include "texture.h"
#include <unordered_set>




class Image_Texture : public Texture {
    public:
    const static int bytes_per_pixel = 3;
    virtual Color getColor(const Point2f &uv) const;
    Image_Texture() : data(nullptr), width(0), height(0), bytes_per_scanline(0) {};
    Image_Texture(const char*  filename);
    ~Image_Texture();
    bool operator==(const Image_Texture& otherImage) const;
    struct HashFunction {
    size_t operator()(const Image_Texture& image) const
        {
            size_t hash = std::hash<std::string>()(image.file_id);
            return hash;
        }
    };
    private:
        const std::string file_id;
        unsigned char *data;
        int width, height;
        int bytes_per_scanline;
};

class Image_Pool {
    public:
    const Image_Texture* getImage(const char*  filename);
    private:
    std::unordered_set<Image_Texture, Image_Texture::HashFunction> images;
};

#endif
