#include "film.h"
#include <fstream>


void Film::FirstLine(const Point2i &resolution, std::ofstream &file) const{
    file << "P3\n" << resolution.x << " " << resolution.y << "\n255\n";
}

void Film::WriteColor(Color c, std::ofstream &file) const{
    int ir = static_cast<int>(255.999 * c.r);
    int ig = static_cast<int>(255.999 * c.g);
    int ib = static_cast<int>(255.999 * c.b);
    file << ir << ' ' << ig << ' ' << ib << '\n';
}

