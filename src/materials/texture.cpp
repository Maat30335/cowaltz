#include "texture.h"
#include "rtw_stb_image.h"
#include "useful.h"

Color SolidColor::getColor(const Point2f &uv) const{
    return color;
}

Image_Texture::Image_Texture(const char* filename){
            auto components_per_pixel = bytes_per_pixel;

            data = stbi_load(filename, &width, &height, &components_per_pixel, components_per_pixel);

            if (!data) {
                std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
                width = height = 0;
            }

            bytes_per_scanline = bytes_per_pixel * width;
}

Image_Texture::~Image_Texture(){
    delete data;
}

Color Image_Texture::getColor(const Point2f &uv) const{
    if(!data){
        return Color(0, 1, 1);
    }

    int i = (int)(clamp(uv.x, 0., 1.) * width);
    int j = (int)(clamp(1 - uv.y, 0., 1.) * height);

    if (i >= width)  i = width - 1;
    if (j >= height) j = height - 1;

    const double scale = 1. / 255.;

    auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;

    return Color(scale * pixel[0], scale * pixel[1], scale * pixel[2]);

}
