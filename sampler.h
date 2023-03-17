#ifndef SAMPLER_H
#define SAMPLER_H

#include "point.h"
#include <vector>

class PixelSampler {

    public:
    PixelSampler(int samplesPerPixels = 1) : samplesPerPixels{samplesPerPixels} {};
    Point2f Get2D();
    void virtual StartPixel() = 0;
    virtual ~PixelSampler() {};

    protected:
    std::vector<Point2f> samples;
    int sampleOffset;
    int samplesPerPixels;

};

class StratifiedSampler : public PixelSampler {
    public:
    StratifiedSampler(int samplesPerPixels = 1) : PixelSampler(samplesPerPixels), samplesPerEdge{(int)sqrt(samplesPerPixels)} {};
    void virtual StartPixel();

    private:
    int samplesPerEdge;
};


#endif
