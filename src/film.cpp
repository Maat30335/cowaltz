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

void Film::WriteColor(Color c, std::ofstream &file) const{
    c.r = std::sqrt(c.r);
    c.g = std::sqrt(c.g);
    c.b = std::sqrt(c.b);
    int ir = static_cast<int>(255.999 * clamp(c.r, 0, 0.999));
    int ig = static_cast<int>(255.999 * clamp(c.g, 0, 0.999));
    int ib = static_cast<int>(255.999 * clamp(c.b, 0, 0.999));
    file << ir << ' ' << ig << ' ' << ib << '\n';
}

