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

    auto carPos = transforms.getTransform(Transform());
   
    std::unique_ptr<Texture> black = std::make_unique<SolidColor>(Color(0, 0, 0));
    std::unique_ptr<Texture> normal = std::make_unique<SolidColor>(Color(0.5, 0.5, 1));


    std::shared_ptr<Material> body = std::make_shared<PrincipledMaterial>(images.getImage("Scenes/CarScene/sovietcar/Textures/body01_albedo.jpg"),
                                                                        images.getImage("Scenes/CarScene/sovietcar/Textures/body01_roughness.jpg"),
                                                                        1.6,
                                                                        images.getImage("Scenes/CarScene/sovietcar/Textures/body01_metallic.jpg"),
                                                                        black.get(),
                                                                        normal.get());
    
    std::shared_ptr<Material> FrWheels = std::make_shared<PrincipledMaterial>(images.getImage("Scenes/CarScene/sovietcar/Textures/frwheel01_albedo.jpg"),
                                                                        images.getImage("Scenes/CarScene/sovietcar/Textures/frwheel01_roughness.jpg"),
                                                                        1.6,
                                                                        images.getImage("Scenes/CarScene/sovietcar/Textures/frwheel01_metallic.jpg"),
                                                                        black.get(),
                                                                        normal.get());

    std::shared_ptr<Material> instruments = std::make_shared<PrincipledMaterial>(images.getImage("Scenes/CarScene/sovietcar/Textures/instr01_albedo.jpg"),
                                                                        images.getImage("Scenes/CarScene/sovietcar/Textures/instr01_roughness.jpg"),
                                                                        1.6,
                                                                        images.getImage("Scenes/CarScene/sovietcar/Textures/instr01_metallic.jpg"),
                                                                        images.getImage("Scenes/CarScene/sovietcar/Textures/instr01_opacity.jpg"),
                                                                        normal.get());

    std::shared_ptr<Material> interior = std::make_shared<PrincipledMaterial>(images.getImage("Scenes/CarScene/sovietcar/Textures/intr01_albedo.jpg"),
                                                                        images.getImage("Scenes/CarScene/sovietcar/Textures/intr01_roughness.jpg"),
                                                                        1.6,
                                                                        images.getImage("Scenes/CarScene/sovietcar/Textures/intr01_metallic.jpg"),
                                                                        black.get(),
                                                                        images.getImage("Scenes/CarScene/sovietcar/Textures/intr01_normal.jpg"));

    std::shared_ptr<Material> CarLights = std::make_shared<PrincipledMaterial>(images.getImage("Scenes/CarScene/sovietcar/Textures/lights01_albedo.jpg"),
                                                                        images.getImage("Scenes/CarScene/sovietcar/Textures/lights01_roughness.jpg"),
                                                                        1.6,
                                                                        images.getImage("Scenes/CarScene/sovietcar/Textures/lights01_metallic.jpg"),
                                                                        black.get(),
                                                                        normal.get());

    std::shared_ptr<Material> roof = std::make_shared<PrincipledMaterial>(images.getImage("Scenes/CarScene/sovietcar/Textures/roof01_albedo.jpg"),
                                                                        images.getImage("Scenes/CarScene/sovietcar/Textures/roof01_roughness.jpg"),
                                                                        1.6,
                                                                        images.getImage("Scenes/CarScene/sovietcar/Textures/roof01_metallic.jpg"),
                                                                        black.get(),
                                                                        images.getImage("Scenes/CarScene/sovietcar/Textures/roof01_normal.jpg"));

    std::shared_ptr<Material> RWheels = std::make_shared<PrincipledMaterial>(images.getImage("Scenes/CarScene/sovietcar/Textures/rwheel01_albedo.jpg"),
                                                                        images.getImage("Scenes/CarScene/sovietcar/Textures/rwheel01_roughness.jpg"),
                                                                        1.6,
                                                                        images.getImage("Scenes/CarScene/sovietcar/Textures/rwheel01_metallic.jpg"),
                                                                        black.get(),
                                                                        normal.get());                                                                    

    std::shared_ptr<Material> StWheel = std::make_shared<PrincipledMaterial>(images.getImage("Scenes/CarScene/sovietcar/Textures/stwheel01_albedo.jpg"),
                                                                        images.getImage("Scenes/CarScene/sovietcar/Textures/stwheel01_roughness.jpg"),
                                                                        1.6,
                                                                        images.getImage("Scenes/CarScene/sovietcar/Textures/stwheel01_metallic.jpg"),
                                                                        black.get(),
                                                                        normal.get());

    std::shared_ptr<Material> glass = std::make_shared<ConstantMaterial>(PrincipledParameters(Color(1, 1, 1), 0.01, 1.6, 0, 1));

    importOBJ::addOBJ(mesh, "Scenes/CarScene/processed/body.obj", carPos.first, carPos.second, body);
    importOBJ::addOBJ(mesh, "Scenes/CarScene/processed/FrontWheels.obj", carPos.first, carPos.second, FrWheels);
    importOBJ::addOBJ(mesh, "Scenes/CarScene/processed/glass.obj", carPos.first, carPos.second, glass);
    importOBJ::addOBJ(mesh, "Scenes/CarScene/processed/instruments.obj", carPos.first, carPos.second, instruments);
    importOBJ::addOBJ(mesh, "Scenes/CarScene/processed/interior.obj", carPos.first, carPos.second, interior);
    importOBJ::addOBJ(mesh, "Scenes/CarScene/processed/Lights.obj", carPos.first, carPos.second, CarLights);
    importOBJ::addOBJ(mesh, "Scenes/CarScene/processed/RearWheels.obj", carPos.first, carPos.second, RWheels);
    importOBJ::addOBJ(mesh, "Scenes/CarScene/processed/roof.obj", carPos.first, carPos.second, roof);
    importOBJ::addOBJ(mesh, "Scenes/CarScene/processed/STwheel.obj", carPos.first, carPos.second, StWheel);

    auto planePos = transforms.getTransform(Translate(Vector3f(-250, 0, -250)));
    std::shared_ptr<Material> floorMat = std::make_shared<ConstantMaterial>(PrincipledParameters(Color(0.15, 0.15, 0.5), 0.7, 1.6, 0, 0));

    std::shared_ptr<Shape> plane = std::make_shared<Plane>(planePos.first, planePos.second, 500, 500);
    std::shared_ptr<Primitive> floor = std::make_shared<GeoPrimitive>(plane, floorMat);
    scene->addPrim(floor);

    auto lightTransform01 = transforms.getTransform(RotateY(30) * RotateZ(70) * Translate(Vector3f(0.5, 4, -0.5)) * RotateZ(180));
    std::shared_ptr<Light> light01 = std::make_shared<RectangleLight>(lightTransform01.first, lightTransform01.second, 1, 1, 10, Color(1, 1, 1));
    theScene.lights.push_back(light01);
    mesh.push_back(light01);

    auto lightTransform02 = transforms.getTransform(RotateZ(-10) * Translate(Vector3f(3.5, 13, -3.5)) * RotateZ(180));
    std::shared_ptr<Light> light02 = std::make_shared<RectangleLight>(lightTransform02.first, lightTransform02.second, 7, 7, 7, Color(1, 1, 1));
    theScene.lights.push_back(light02);
    mesh.push_back(light02);

    auto lightTransform03 = transforms.getTransform(RotateY(180) * RotateZ(-10) * Translate(Vector3f(3, 10, -3)) * RotateZ(180));
    std::shared_ptr<Light> light03 = std::make_shared<RectangleLight>(lightTransform03.first, lightTransform03.second, 6, 6, 5, Color(1, 1, 1));
    theScene.lights.push_back(light03);
    mesh.push_back(light03);

    auto lightTransform04 = transforms.getTransform(Translate(Vector3f(-0.2, 0.49, 0)) * RotateX(180));
    std::shared_ptr<Light> light04 = std::make_shared<RectangleLight>(lightTransform04.first, lightTransform04.second, 0.4, 0.1, 1, Color(1, 1, 1));
    theScene.lights.push_back(light04);
    mesh.push_back(light04);



    std::shared_ptr<Primitive> BVHmesh = BVHnode::createBVH(mesh);
    scene->addPrim(BVHmesh);

    
    
    


    std::shared_ptr<Film> film = std::make_shared<Film>(resolution);

    
    Transform cameraToWorld = LookAt(Point3f(2, 0.75, 2), Point3f(0, 0.2, 0.2), Vector3f(0, 1, 0));
    std::shared_ptr<Camera> camera = std::make_shared<PerspectiveCamera>(&cameraToWorld, resolution, Radians(45), 4, 0);
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
