#include "transform.h"
#include "sphere.h"
#include "camera.h"
#include <fstream>
#include "film.h"
#include "primitive.h"
#include <memory>
#include "integrator.h"
#include "sampler.h"
#include <iostream>


int main(){
    Transform SpherePos = Translate(Vector3f(0, 0.5, -5));
    Transform SpherePosInv = Inv(SpherePos);
    Transform SpherePos2 = Translate(Vector3f(0, -10, -5));
    Transform SpherePosInv2 = Inv(SpherePos2);
    Transform Identity;
    const Point2i resolution{256, 256};




    std::shared_ptr<Shape> sphere1_shape = std::make_shared<Sphere>(&SpherePos, &SpherePosInv, 1.0);
    std::shared_ptr<Shape> sphere2_shape = std::make_shared<Sphere>(&SpherePos2, &SpherePosInv2, 9.0);
    std::shared_ptr<Primitive> sphere1_prim = std::make_shared<GeoPrimitive>(sphere1_shape);
    std::shared_ptr<Primitive> sphere2_prim = std::make_shared<GeoPrimitive>(sphere2_shape);

    PrimitiveList scene;
    scene.addPrim(sphere1_prim);
    scene.addPrim(sphere2_prim);

    std::shared_ptr<Film> film = std::make_shared<Film>();
    std::shared_ptr<Camera> camera = std::make_shared<PerspectiveCamera>(&Identity, resolution, 250);
    std::shared_ptr<PixelSampler> sampler = std::make_shared<StratifiedSampler>(1024);
    std::cout << sampler->samplesPerPixel << std::endl;
    
    LambertIntegrator pog{film, camera, sampler};
    pog.Render(scene);
    


}
