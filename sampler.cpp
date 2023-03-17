#include "sampler.h"
#include "useful.h"
#include <iostream>


Point2f PixelSampler::Get2D(){
    if(sampleOffset > samples.size()){
        std::cout << "bro fix ur sampling" << std::endl;
        sampleOffset++;
        return Point2f(random_float(), random_float());
    }else{
        sampleOffset++;
        return samples[sampleOffset - 1];

    }
}

void StratifiedSampler::StartPixel(){
    samples.clear();
    float d = (float)1 / samplesPerEdge;
    for(int y = 0; y < samplesPerEdge; ++y){
        for(int x = 0; x < samplesPerEdge; ++x){
            float jx = random_float();
            float jy = random_float();
            samples.push_back(Point2f((x + jx) * d, (y + jy) * d));
        }
    }
}


