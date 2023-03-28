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




int main(){

   

    PrimitiveList scene;

    Transform SpherePos = Translate(Vector3f(0, .5, -3)) * Scale(0.5);
    Transform SpherePosInv = Inv(SpherePos);
    
    Transform SpherePos2 = Translate(Vector3f(0, -100, -3));
    Transform SpherePosInv2 = Inv(SpherePos2);
    Transform Identity;
    const Point2i resolution{512, 512};

    std::shared_ptr<Material> floor = std::make_shared<PrincipledShader>(Color(0.8, 0.9, 1), 0.5, 1.5);
    std::shared_ptr<Material> ball = std::make_shared<PrincipledShader>(Color(0.3, 0, 0.9), 0.5, 2);


    std::shared_ptr<Shape> sphere_shape = std::make_shared<Sphere>(&SpherePos, &SpherePosInv, 1);
    std::shared_ptr<Primitive> sphere_prim = std::make_shared<GeoPrimitive>(sphere_shape, ball);

    std::shared_ptr<Shape> sphere2_shape = std::make_shared<Sphere>(&SpherePos2, &SpherePosInv2, 99.0);
    std::shared_ptr<Primitive> sphere2_prim = std::make_shared<GeoPrimitive>(sphere2_shape, floor);

    std::unique_ptr<Point3f[]> verts;
    std::unique_ptr<int[]> vertexIndices;
    int nVertices;
    int nTriangles;



    importOBJ::loadOBJ("stuff/cow.obj", verts, vertexIndices, &nVertices, &nTriangles);
    
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
    scene.addPrim(BVHmesh);


    // scene.addPrim(mesh);

    scene.addPrim(sphere2_prim);

    std::shared_ptr<Film> film = std::make_shared<Film>();
    std::shared_ptr<Camera> camera = std::make_shared<PerspectiveCamera>(&Identity, resolution, 200);
    std::shared_ptr<PixelSampler> sampler = std::make_shared<StratifiedSampler>(625);
    std::cout << sampler->samplesPerPixel << std::endl;
    
    LambertIntegrator pog{film, camera, sampler};
    std::cout << "Rendering... " << std::endl;
    pog.Render(scene);
    


}
