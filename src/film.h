#ifndef FILM_H
#define FILM_H

#include <string>
#include "spectrum.h"
#include "point.h"
#include <memory>

class Film {
    public:
    Film(const Point2i &resolution);
    ~Film();
    void FirstLine(const Point2i &resolution, std::ofstream &file) const;
    void WriteColor(Color c, const Point2i &pixel);
    void WriteFile(const std::string &filename);
    void WriteColor(Color c, std::ofstream &file) const;
    const Point2i resolution;
    private:
    Color** image;

};



#endif
