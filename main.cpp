#include "transform.h"
#include "sphere.h"
#include "camera.h"
#include <fstream>
#include "film.h"
#include "primitive.h"
#include <memory>


int main(){
    Transform SpherePos = Translate(Vector3f(0, 0, -5));
    Transform SpherePosInv = Inv(SpherePos);
    Transform SpherePos2 = Translate(Vector3f(0, 4, -5));
    Transform SpherePosInv2 = Inv(SpherePos2);
    Transform Identity;
    const Point2i resolution{256, 256};
    std::ofstream file("test.ppm");
    Film f{"test.ppm"};
    f.FirstLine(resolution, file);




    std::shared_ptr<Shape> sphere1_shape = std::make_shared<Sphere>(&SpherePos, &SpherePosInv, 1.0);
    std::shared_ptr<Shape> sphere2_shape = std::make_shared<Sphere>(&SpherePos2, &SpherePosInv2, 2.0);
    std::shared_ptr<Primitive> sphere1_prim = std::make_shared<GeoPrimitive>(sphere1_shape);
    std::shared_ptr<Primitive> sphere2_prim = std::make_shared<GeoPrimitive>(sphere2_shape);

    PrimitiveList scene;
    scene.addPrim(sphere1_prim);
    scene.addPrim(sphere2_prim);

    PerspectiveCamera cam = PerspectiveCamera(&Identity, (Point2f)resolution, 1);

    for(int j = resolution.y - 1; j >= 0; j--){
        for(int i = 0; i < resolution.x; i++){
            Point2f sample{j / (float)resolution.y, i / (float)resolution.x};
            Ray r = cam.GenerateRay(sample);
            if(scene.Intersect(r, nullptr)){
                f.WriteColor(Color{1, 0, 0}, file);
            }else{
                f.WriteColor(Color{0, 0, 1}, file);
            }
        }
    }


}
