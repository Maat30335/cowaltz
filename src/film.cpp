#include "film.h"
#include <fstream>
#include "useful.h"
#include <cmath>

Film::Film(const Point2i &resolution) : resolution{resolution} {
    image = new Color*[resolution.x];
    for(int i = 0; i < resolution.x; i++){
        image[i] = new Color[resolution.y];
    }
}

Film::~Film(){
    for(int i = 0; i < resolution.x; i++){
        delete[] image[i];
    }
    delete[] image;
}

void Film::WriteColor(Color c, const Point2i &pixel){
    image[pixel.x][pixel.y] = c;
}

void Film::WriteFile(const std::string &filename){
    std::ofstream file("test.ppm");
    FirstLine(resolution, file);
    for (int j = resolution.y; j >= 0; --j) {
        for (int i = 0; i < resolution.x; i++){
            WriteColor(image[i][j], file);
        }
    }
}

void Film::FirstLine(const Point2i &resolution, std::ofstream &file) const{
    file << "P3\n" << resolution.x << " " << resolution.y << "\n255\n";
}

void Film::WriteColor(Color color, std::ofstream &file) const{

    color.r = std::sqrt(color.r);
    color.g = std::sqrt(color.g);
    color.b = std::sqrt(color.b);
    int ir = static_cast<int>(255.999 * clamp(color.r, 0, 0.999));
    int ig = static_cast<int>(255.999 * clamp(color.g, 0, 0.999));
    int ib = static_cast<int>(255.999 * clamp(color.b, 0, 0.999));
    file << ir << ' ' << ig << ' ' << ib << '\n';
}

