#ifndef USEFUL_H
#define USEFUL_H

#include <stdexcept>
#include <string>
#include <cmath>
#include <random>

static const float OneMinusEpsilon = 0x1.fffffep-1;

inline void Error(std::string s){
    throw std::runtime_error(s);
}

inline bool Quadratic(float a, float b, float c, float *t0, float *t1){
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

inline float random_float() {
    static std::uniform_real_distribution<float> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}



#endif
