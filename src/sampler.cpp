#include "sampler.h"
#include "useful.h"
#include <iostream>



Point2f PixelSampler::Get2D(){
    if(sampleOffset > samples.size()){
        sampleOffset++;
        std::cout << "fix ur sampling bro cuz " << sampleOffset << " > " << samples.size() << std::endl;
        return Point2f(random_double(), random_double());
    }else{
        sampleOffset++;
        return samples[sampleOffset - 1];

    }
}

StratifiedSampler::StratifiedSampler(int samplesPerPixel) : 
    PixelSampler(samplesPerPixel), samplesPerEdge{(int)sqrt(samplesPerPixel)}
    {
    std::cout << samplesPerEdge << std::endl;
    samplesPerPixel = samplesPerEdge * samplesPerEdge;
}

void StratifiedSampler::StartPixel(){
    samples.clear();
    sampleOffset = 0;
    // std::cout << samplesPerPixels << " vs " << samplesPerEdge << std::endl;
    double d = (double)1 / samplesPerEdge;
    for(int y = 0; y < samplesPerEdge; ++y){
        for(int x = 0; x < samplesPerEdge; ++x){
            double jx = random_double();
            double jy = random_double();
            samples.push_back(Point2f((x + jx) * d, (y + jy) * d));
        }
    }
}

std::unique_ptr<PixelSampler> StratifiedSampler::Clone() const{
    std::unique_ptr<PixelSampler> out = std::make_unique<StratifiedSampler>(*this);
    return out;
}


