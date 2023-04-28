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
#include "point.h"
#include "triangle.h"
#include "material.h"
#include "importobj.h"
#include "bvh.h"
#include "disney.h"
#include "parameters.h"
#include "useful.h"
#include "plane.h"
#include "scene.h"
#include "light.h"
#include "texture.h"
#include "image.h"
#include <chrono>




int main(){

   

    std::shared_ptr<PrimitiveList> scene = std::make_shared<PrimitiveList>();
    Image_Pool images;
    Transform_Pool transforms;
    Scene theScene;
    std::vector<std::shared_ptr<Primitive>> mesh;
    
    
    
    const Point2i resolution{512, 512};

    auto identity = transforms.getTransform(Transform());

    std::shared_ptr<Material> red = std::make_shared<ConstantMaterial>(PrincipledParameters(Color(0.65, 0.05, 0.05), 1, 1.6, 0, 0));
    std::shared_ptr<Material> green = std::make_shared<ConstantMaterial>(PrincipledParameters(Color(0.12, 0.45, 0.15), 1, 1.6, 0, 0));
    std::shared_ptr<Material> white = std::make_shared<ConstantMaterial>(PrincipledParameters(Color(0.73, 0.73, 0.73), 1, 1.6, 0, 0));
    std::shared_ptr<Material> cow = std::make_shared<ConstantMaterial>(PrincipledParameters(Color(1., 1., 1.), 0.2, 1.6, 1, 0));
    std::shared_ptr<Material> bunny = std::make_shared<ConstantMaterial>(PrincipledParameters(Color(0.15, 0.15, 0.6), 0.1, 1.6, 0, 0));


    importOBJ::addOBJ(mesh, "Scenes/CornellBox/whitewall.obj", identity.first, identity.second, white);
    importOBJ::addOBJ(mesh, "Scenes/CornellBox/redwall.obj", identity.first, identity.second, red);
    importOBJ::addOBJ(mesh, "Scenes/CornellBox/greenwall.obj", identity.first, identity.second, green);
    importOBJ::addOBJ(mesh, "Scenes/CornellBox/cow.obj", identity.first, identity.second, cow);
    importOBJ::addOBJ(mesh, "Scenes/CornellBox/bunny.obj", identity.first, identity.second, bunny);


    auto lightTransform = transforms.getTransform(Translate(Vector3f(0.25, 1.56, -0.25)) * RotateZ(180));
    std::shared_ptr<Light> light = std::make_shared<RectangleLight>(lightTransform.first, lightTransform.second, 0.5, 0.5, 30, Color(1, 1, 1));
    theScene.lights.push_back(light);
    mesh.push_back(light);



    std::shared_ptr<Primitive> BVHmesh = BVHnode::createBVH(mesh);
    scene->addPrim(BVHmesh);

    
    
    


    std::shared_ptr<Film> film = std::make_shared<Film>(resolution);

    
    Transform cameraToWorld = LookAt(Point3f(0, 0.5, 2), Point3f(0, 0.65, 0.), Vector3f(0, 1, 0));
    std::shared_ptr<Camera> camera = std::make_shared<PerspectiveCamera>(&cameraToWorld, resolution, Radians(60), 4, 0);
    std::shared_ptr<PixelSampler> sampler = std::make_shared<StratifiedSampler>(4096);
    std::cout << sampler->samplesPerPixel << std::endl;
    
    DisneyIntegrator pog{film, camera, sampler};
    std::cout << "Rendering... " << std::endl;
    
    theScene.scene = scene;


    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    //pog.Render(theScene);
    pog.MultiRender(theScene);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Render Time = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[sec]" << std::endl;
    


}
