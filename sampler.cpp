#include "sampler.h"
#include "useful.h"
#include <iostream>



Point2f PixelSampler::Get2D(){
    if(sampleOffset > samples.size()){
        sampleOffset++;
        std::cout << "fix ur sampling bro cuz " << sampleOffset << " > " << samples.size() << std::endl;
        return Point2f(random_float(), random_float());
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
    float d = (float)1 / samplesPerEdge;
    for(int y = 0; y < samplesPerEdge; ++y){
        for(int x = 0; x < samplesPerEdge; ++x){
            float jx = random_float();
            float jy = random_float();
            samples.push_back(Point2f((x + jx) * d, (y + jy) * d));
        }
    }
}


