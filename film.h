#ifndef FILM_H
#define FILM_H

#include <string>
#include "spectrum.h"
#include "point.h"

class Film {
    public:
    Film(std::string filename) : filename{filename} {};
    void FirstLine(const Point2i &resolution, std::ofstream &file) const;
    void WriteColor(Color c, std::ofstream &file) const;
    // const Point2i fullResolution;
    const std::string filename;

};



#endif
