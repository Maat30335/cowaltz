#include "spectrum.h"
#include <cassert>

Color Color::operator+(const Color &v) const{
        return Color(r + v.r, g + v.g, b + v.b);
    }

    Color& Color::operator+=(const Color &v){
        r += v.r;
        g += v.g;
        b += v.b;
        return *this;
    }

    Color Color::operator-(const Color &v) const{
        return Color(r - v.r, g - v.g, b - v.b);
    }
        
    Color& Color::operator-=(const Color &v){
        r -= v.r;
        g -= v.g;
        b -= v.b;
        return *this;
    }

    bool Color::operator==(const Color &v) const{
        return r == v.r && g == v.g && b == v.b;
    }

    bool Color::operator!=(const Color &v) const{
        return r != v.r || g != v.g || b != v.b;
    }

    Color Color::operator*(float s) const{
        return Color(s * r, s * g, s * b);
    }

    Color& Color::operator*=(float s){
        r *= s;
        g *= s;
        b *= s;
        return *this;
    }

    Color Color::operator/(float f) const{
        assert(f != 0);
        float inv = (float) 1 / f;
        return Color(inv * r, inv * g, inv * b);
    }

    Color& Color::operator/=(float f){
        assert(f != 0);
        float inv = (float) 1 / f;
        r *= inv;
        g *= inv;
        b *= inv;
        return *this;
    }
