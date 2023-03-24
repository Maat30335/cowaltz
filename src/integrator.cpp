#include "integrator.h"
#include <fstream>
#include <iostream>
#include "interaction.h"
#include "useful.h"
#include "point.h"
#include "vector.h"

void SamplerIntegrator::Render(const Primitive &scene){
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
                
                c = c + rayColor(r, scene, 2);
                
            }
            c = c * scale;
            film->WriteColor(c, file);
        }
    }
}

Color RedIntegrator::rayColor(const Ray &r, const Primitive &scene, int depth) const{
    SurfaceInteraction isect;
    if(scene.Intersect(r, &isect)){
                return Color{1, 0, 0};
            }else{
                return Color{0, 0, 1};
            }
}

Color LambertIntegrator::rayColor(const Ray &r, const Primitive &scene, int depth) const{
    if(depth <= 0){
        return Color(0, 0, 0);
    }
    SurfaceInteraction isect;
    if(scene.Intersect(r, &isect)){
                Vector3f d = (Vector3f)isect.n + Normalize(randomInSphere());
                return 0.5 * rayColor(Ray(isect.p, d), scene, depth - 1);
            }else{

                Vector3f unit_direction = Normalize(r.d);
                auto t = 0.5*(unit_direction.y + 1.0);
                return (1.0-t)*Color(1.0, 1.0, 1.0) + t*Color(0.5, 0.7, 1.0);
            }

}

Color NormalIntegrator::rayColor(const Ray &r, const Primitive &scene, int depth) const{
    SurfaceInteraction isect;
        if(scene.Intersect(r, &isect)){
                isect.n = Normalize(isect.n);
                return 0.5 * Color{isect.n.x + 1, isect.n.y + 1, isect.n.z + 1};
            }else{
                Vector3f unit_direction = Normalize(r.d);
                auto t = 0.5*(unit_direction.y + 1.0);
                return (1.0-t)*Color(1.0, 1.0, 1.0) + t*Color(0.5, 0.7, 1.0);
            }
}
