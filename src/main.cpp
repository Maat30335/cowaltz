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
#include "normal.h"
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




int main(){

   

    std::shared_ptr<PrimitiveList> scene = std::make_shared<PrimitiveList>();

    Transform SpherePos = Translate(Vector3f(0, -1, -2));
    Transform SpherePosInv = Inv(SpherePos);
    
    
    
    const Point2i resolution{256, 256};

   
    std::shared_ptr<Material> ball = std::make_shared<ConstantMaterial>(PrincipledParameters(Color(0.9, 0., 0.), 0.001, 1.6, 0., 1.));


    std::shared_ptr<Shape> sphere_shape = std::make_shared<Sphere>(&SpherePos, &SpherePosInv, 1);
    std::shared_ptr<Primitive> sphere_prim = std::make_shared<GeoPrimitive>(sphere_shape, ball);

    

    std::unique_ptr<Point3f[]> verts;
    std::unique_ptr<int[]> vertexIndices;
    int nVertices;
    int nTriangles;



    importOBJ::loadOBJ("stuff/wall.obj", verts, vertexIndices, &nVertices, &nTriangles);
    
    std::vector<std::shared_ptr<Shape>> triangles = CreateTriangleMesh(&SpherePos, &SpherePosInv, 
            nTriangles, vertexIndices.get(), nVertices, verts.get(), nullptr, nullptr);


    //std::shared_ptr<PrimitiveList> mesh = std::make_shared<PrimitiveList>();
    std::vector<std::shared_ptr<Primitive>> mesh;

    for(int i = 0; i < nTriangles; i++){
        std::shared_ptr<Primitive> tri = std::make_shared<GeoPrimitive>(triangles[i], ball);
        // mesh->addPrim(tri);
        mesh.push_back(tri);
    }

    std::shared_ptr<Primitive> BVHmesh = BVHnode::createBVH(mesh);
    // scene->addPrim(BVHmesh);


    // scene.addPrim(sphere_prim);
    Transform SpherePos2 = Translate(Vector3f(-100, -1.01, -100));
    Transform SpherePosInv2 = Inv(SpherePos2);
    std::shared_ptr<Material> floor = std::make_shared<ConstantMaterial>(PrincipledParameters(Color(0.5, 0.5, 0.5), 0.5, 1.5, 0., 0.));
    std::shared_ptr<Shape> sphere2_shape = std::make_shared<Plane>(&SpherePos2, &SpherePosInv2, 200, 200);
    std::shared_ptr<Primitive> sphere2_prim = std::make_shared<GeoPrimitive>(sphere2_shape, floor);
    //scene->addPrim(sphere2_prim);

    
    
    Transform SpherePos3 = Translate(Vector3f(1, 0, -4));
    Transform SpherePosInv3 = Inv(SpherePos3);
    std::shared_ptr<Shape> sphere3_shape = std::make_shared<Sphere>(&SpherePos3, &SpherePosInv3, 1);
    std::shared_ptr<Material> ball2 = std::make_shared<ConstantMaterial>(PrincipledParameters(Color(1, 0, 0), 1, 1.6, 0., 0.));
    std::shared_ptr<Primitive> sphere3_prim = std::make_shared<GeoPrimitive>(sphere3_shape, ball2);
    scene->addPrim(sphere3_prim);
    


    std::shared_ptr<Film> film = std::make_shared<Film>();
    
    Transform cameraToWorld; // = LookAt(Point3f(4, 4, -5), Point3f(0, 0, -2), Vector3f(0, 1, 0));
    std::shared_ptr<Camera> camera = std::make_shared<PerspectiveCamera>(&cameraToWorld, resolution, Radians(90.), 4, 0);
    std::shared_ptr<PixelSampler> sampler = std::make_shared<StratifiedSampler>(256);
    std::cout << sampler->samplesPerPixel << std::endl;


    Transform lightPos = Translate(Vector3f(-1, -1.5, -3));
    Transform lightPosInv = Inv(lightPos);
    std::shared_ptr<Light> light = std::make_shared<RectangleLight>(&lightPos, &lightPosInv, 2, 2, 10, Color(1, 1, 1));
    std::shared_ptr<Primitive> recLight = light;

    std::shared_ptr<Shape> fakelight = std::make_shared<Plane>(&lightPos, &lightPosInv, 2, 2);
    std::shared_ptr<Primitive> fakelightPrim = std::make_shared<GeoPrimitive>(fakelight, floor);

    //scene->addPrim(fakelightPrim);
    scene->addPrim(recLight);
    
    DisneyIntegrator pog{film, camera, sampler};
    std::cout << "Rendering... " << std::endl;
    Scene theScene;
    theScene.scene = scene;
    theScene.lights.push_back(light);
    pog.Render(theScene);
    


}
