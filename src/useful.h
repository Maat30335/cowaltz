#ifndef USEFUL_H
#define USEFUL_H

#include <stdexcept>
#include <string>
#include <cmath>
#include <random>
#include "point.h"


static const double OneMinusEpsilon = 0x1.fffffep-1;
static const double Epsilon = 0.00000000001;
static const double Pi = 3.1415926;

inline void Error(std::string s){
    throw std::runtime_error(s);
}

inline bool Quadratic(double a, double b, double c, double *t0, double *t1){
    double discrim = (double)b * (double)b - 4 * (double)a * (double)c;
    if (discrim < 0) return false;
    double rootDiscrim = std::sqrt(discrim);

    double q;
    if (b < 0) q = -.5 * (b - rootDiscrim);
    else       q = -.5 * (b + rootDiscrim);
    *t0 = q / a;
    *t1 = c / q;
    if (*t0 > *t1) std::swap(*t0, *t1);
    return true;
}

inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline Vector3f randomInSphere(){
    while(true){
        Vector3f v = Vector3f(random_double(), random_double(), random_double());
        if(v.NormSquared() >= 1) continue;
        return v;
    }
}

inline double clamp(double x, double min, double max){
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

inline double Radians(double theta){
    return theta * Pi / 180;
}




#endif
