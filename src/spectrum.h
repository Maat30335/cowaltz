#ifndef SPECRUM_H
#define SPECRUM_H

#include <iostream>


class Color {
    public:
    Color() : r{0.}, g{0.}, b{0.} {};
    Color(double r, double g, double b) : r{r}, g{g}, b{b} {};
    double r, g, b;
    Color operator+(const Color &v) const;
    Color operator+(double d) const;
    Color& operator+=(const Color &v);
    Color operator-(const Color &v) const;
    Color& operator-=(const Color &v);
    bool operator==(const Color &v) const;
    bool operator!=(const Color &v) const;
    Color operator*(double s) const;
    Color operator*(const Color &v) const;
    Color operator/(const Color &v) const;
    Color& operator*=(double s);
    Color operator/(double f) const;
    Color& operator/=(double f);
    Color operator-() const;

};

inline Color operator*(double s,const Color &v) {
    return v * s;
}

inline Color operator+(double d, const Color &v){
    return v + d;
}


inline std::ostream& operator<<(std::ostream &out, const Color &v) {
    return out << v.r << ' ' << v.g << ' ' << v.b;
}






#endif
