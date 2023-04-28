#include "image.h"
#include "rtw_stb_image.h"
#include "useful.h"

Image_Texture::Image_Texture(const char* filename) : file_id{std::string(filename)} {
            auto components_per_pixel = bytes_per_pixel;

            data = stbi_load(filename, &width, &height, &components_per_pixel, components_per_pixel);

            if (!data) {
                std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
                exit(1);
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


    int i = (int)((uv.x - floor(uv.x)) * width);
    int j = (int)(((1 - uv.y) - floor(1 - uv.y)) * height);

    if (i >= width)  i = width - 1;
    if (j >= height) j = height - 1;

    const double scale = 1. / 255.;

    auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;

    return Color(scale * pixel[0], scale * pixel[1], scale * pixel[2]);

}

bool Image_Texture::operator==(const Image_Texture& otherImage) const{
    return this->file_id == otherImage.file_id;
}

const Image_Texture* Image_Pool::getImage(const char* filename){
    const Image_Texture* image = &*images.emplace(filename).first; 
    return image;
}
