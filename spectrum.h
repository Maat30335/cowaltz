#ifndef SPECRUM_H
#define SPECRUM_H

#include <iostream>


class Color {
    public:
    Color(float r, float g, float b) : r{r}, g{g}, b{b} {};
    float r, g, b;
    Color operator+(const Color &v) const;
    Color& operator+=(const Color &v);
    Color operator-(const Color &v) const;
    Color& operator-=(const Color &v);
    bool operator==(const Color &v) const;
    bool operator!=(const Color &v) const;
    Color operator*(float s) const;
    Color& operator*=(float s);
    Color operator/(float f) const;
    Color& operator/=(float f);

};

inline Color operator*(float s,const Color &v) {
    return v * s;
}


inline std::ostream& operator<<(std::ostream &out, const Color &v) {
    return out << v.r << ' ' << v.g << ' ' << v.b;
}




#endif
