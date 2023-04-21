#include "integrator.h"
#include <fstream>
#include <iostream>
#include "interaction.h"
#include "useful.h"
#include "point.h"
#include "vector.h"
#include "disney.h"


void SamplerIntegrator::Render(const Scene &scene){
    std::ofstream file("test.ppm");
    film->FirstLine(camera->resolution, file);
    int imageHeight = camera->resolution.y;
    int imageWidth = camera->resolution.x;
    double scale = (double)1 / sampler->samplesPerPixel;
    for (int j = imageHeight-1; j >= 0; --j) {
        std::cout << "Scanline... " << j << std::endl;
        for (int i = 0; i < imageWidth; ++i) {
            // std::cout << "Pixel... " << i << std::endl;
            sampler->StartPixel();
            Color c{0, 0, 0};
            for(int k = 0; k < sampler->samplesPerPixel; k++){
                Point2f sample = sampler->Get2D();
                // std::cout << "sample: " << sample.x << ", " << sample.y << std::endl;
                // std::cout << "Point2i: " << i << ", " << j << std::endl;
                Ray r = camera->GenerateRay(sample, Point2i(i, j));
                
                c = c + rayColor(r, scene, 4);
                
            }
            c = c * scale;
            film->WriteColor(c, file);
        }
    }
}

Color RedIntegrator::rayColor(const Ray &r, const Scene &scene, int depth) const{
    SurfaceInteraction isect;
    if(scene.scene->Intersect(r, &isect)){
                return Color{1, 0, 0};
            }else{
                return Color{0, 0, 1};
            }
}

Color LambertIntegrator::rayColor(const Ray &r, const Scene &scene, int depth) const{
    Color L{0, 0, 0};
    Color T{1, 1, 1};

    Ray ray = r;

    for(int i = 0; i < depth; i++){
        SurfaceInteraction isect;
        if(scene.scene->Intersect(ray, &isect)){
                Vector3f wi = Normalize((Vector3f)isect.n + Normalize(randomInSphere()));
                Vector3f wo = Normalize(ray.d);
                T *= 0.5;
                ray = Ray(isect.p + Epsilon * (Vector3f)isect.n, wi);
        }else{
                
                Vector3f unit_direction = Normalize(ray.d);
                auto t = 0.5*(unit_direction.y + 1.0);
                L = T * ((1.0-t)*Color(1.0, 1.0, 1.0) + t*Color(0.5, 0.7, 1.0));
                return L;
        }
    }

    return L;

}


Color DisneyIntegrator::rayColor(const Ray &r, const Scene &scene, int depth) const{


    Color L{0, 0, 0};
    Color T{1, 1, 1};

    Ray ray = r;
    double oldPDF = 0;
    bool oldDirac = true;

    for(int i = 0; i < depth; i++){
        SurfaceInteraction isect;
        if(scene.scene->Intersect(ray, &isect)){
            BSDFSample sample;
            Vector3f wo = Normalize(-ray.d);
            sample = Disney::SampleDisney(isect, wo);
            Vector3f wi = sample.wi;
            if(sample.isBlack){
                    return Color(0, 0, 0);
            }
            Point3f pHit = sample.hitIn ? isect.p + Epsilon * -(Vector3f)isect.n : isect.p + Epsilon * (Vector3f)isect.n;
            bool isDirac = (1 - isect.parameters.metallic) * (1 - isect.parameters.specTrans) == 0. && isect.parameters.roughness <= 0.01;

            if(isect.light){
                Color emit = oldDirac ? isect.light->getEmittance() : isect.light->getEmittance() * oldPDF / (oldPDF + isect.light->pdf(wi, ray.o, pHit));
                L += T * emit;
                // std::cout << "light?: " << L << std::endl;

            } else if(!scene.lights.empty() && !isDirac){ // sampling light

                int index = (int)(random_double() * scene.lights.size());
                LightSample lSample = scene.lights[index]->Sample_Li(isect.p);
                Ray lightRay{pHit, lSample.wi, lSample.tHit - Epsilon};
                if(!lSample.isBlack && !scene.scene->IntersectP(lightRay)){
                    double pdf;
                    Color emit = Disney::EvaluateDisney(isect, wi, wo, &pdf);
                    L += T * (emit / (pdf + lSample.pdf));
                }

            }
            
            T = T * sample.reflectance;
            oldDirac = isDirac;
            oldPDF = sample.pdf;
            ray = Ray(pHit, wi);
        }else{
            Vector3f unit_direction = Normalize(ray.d);
                auto t = 0.5*(unit_direction.y + 1.0);
                L += T * ((1.0-t)*Color(1.0, 1.0, 1.0) + t*Color(0.5, 0.7, 1.0));
                return L;
        }
    }
    return L;
}

    






/*
    if(depth <= 0){
        return Color(0, 0, 0);
    }
    SurfaceInteraction isect;
    if(scene.scene->Intersect(r, &isect)){
                BSDFSample sample;
                Vector3f wo = Normalize(-r.d);
                sample = Disney::SampleDisney(isect, wo);
                Vector3f wi = sample.wi;
                if(sample.isBlack){
                    return Color(0, 0, 0);
                }
                Point3f pHit = sample.hitIn ? isect.p + Epsilon * -(Vector3f)isect.n : isect.p + Epsilon * (Vector3f)isect.n;
                return sample.reflectance * rayColor(Ray(pHit, wi), scene, depth - 1);
            }else{

                Vector3f unit_direction = Normalize(r.d);
                auto t = 0.5*(unit_direction.y + 1.0);
                return (1.0-t)*Color(1.0, 1.0, 1.0) + t*Color(0.5, 0.7, 1.0);
            }

}
*/


Color NormalIntegrator::rayColor(const Ray &r, const Scene &scene, int depth) const{
    SurfaceInteraction isect;
    Ray ray = r;
        if(scene.scene->Intersect(ray, &isect)){
                isect.n = Normalize(isect.n);
                return 0.5 * Color{isect.n.x + 1, isect.n.y + 1, isect.n.z + 1};
            }else{
                Vector3f unit_direction = Normalize(ray.d);
                auto t = 0.5*(unit_direction.y + 1.0);
                return (1.0-t)*Color(1.0, 1.0, 1.0) + t*Color(0.5, 0.7, 1.0);
            }
}
