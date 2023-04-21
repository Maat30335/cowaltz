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
#include "texture.h"




int main(){

   

    std::shared_ptr<PrimitiveList> scene = std::make_shared<PrimitiveList>();

    
    
    
    
    const Point2i resolution{512, 512};

   
    

    

    std::unique_ptr<Point3f[]> verts;
    std::unique_ptr<Normal3f[]> normals;
    std::unique_ptr<Point2f[]> uvs;
    std::unique_ptr<int[]> vertexIndices;
    int nVertices;
    int nTriangles;

    importOBJ::loadOBJ("stuff/cube.obj", verts, normals, uvs, vertexIndices, nVertices, nTriangles);

    Transform OBJPos = Translate(Vector3f(0, -.5, -1)) * RotateX(-80);
    Transform OBJPosInv = Inv(OBJPos);
    
    std::vector<std::shared_ptr<Shape>> triangles = CreateTriangleMesh(&OBJPos, &OBJPosInv, 
            nTriangles, vertexIndices.get(), nVertices, verts.get(), normals.get(), uvs.get());


    std::vector<std::shared_ptr<Primitive>> mesh;

    std::unique_ptr<Texture> albedo = std::make_unique<Image_Texture>("stuff/tj4kedvcw_2K_Albedo.jpg");
    std::unique_ptr<Texture> normal = std::make_unique<SolidColor>(Color(0.5, 0.5, 1));
    // std::unique_ptr<Texture> normal = std::make_unique<Image_Texture>("stuff/tj4kedvcw_2K_Normal.jpg");
    std::unique_ptr<Texture> roughness = std::make_unique<Image_Texture>("stuff/tj4kedvcw_2K_Roughness.jpg");
    std::unique_ptr<Texture> specTrans = std::make_unique<SolidColor>(Color(0, 0, 0));
    std::unique_ptr<Texture> metallic = std::make_unique<SolidColor>(Color(0, 0, 0));

    std::shared_ptr<Material> objMat = std::make_shared<PrincipledMaterial>(albedo.get(), roughness.get(), 1.6, metallic.get(), specTrans.get(), normal.get());

    for(int i = 0; i < nTriangles; i++){
        std::shared_ptr<Primitive> tri = std::make_shared<GeoPrimitive>(triangles[i], objMat);
        mesh.push_back(tri);
    }

    std::shared_ptr<Primitive> BVHmesh = BVHnode::createBVH(mesh);
    scene->addPrim(BVHmesh);


    Transform SpherePos2 = Translate(Vector3f(-100, -1, -100));
    Transform SpherePosInv2 = Inv(SpherePos2);
    std::shared_ptr<Material> floor = std::make_shared<ConstantMaterial>(PrincipledParameters(Color(0.5, 0.5, 0.5), 0.5, 1.5, 0., 0.));
    std::shared_ptr<Shape> sphere2_shape = std::make_shared<Plane>(&SpherePos2, &SpherePosInv2, 200, 200);
    std::shared_ptr<Primitive> sphere2_prim = std::make_shared<GeoPrimitive>(sphere2_shape, floor);
    // scene->addPrim(sphere2_prim);

    
    


    std::shared_ptr<Film> film = std::make_shared<Film>();
    
    Transform cameraToWorld; // = LookAt(Point3f(4, 4, -5), Point3f(0, 0, -2), Vector3f(0, 1, 0));
    std::shared_ptr<Camera> camera = std::make_shared<PerspectiveCamera>(&cameraToWorld, resolution, Radians(90.), 4, 0);
    std::shared_ptr<PixelSampler> sampler = std::make_shared<StratifiedSampler>(100);
    std::cout << sampler->samplesPerPixel << std::endl;
    
    DisneyIntegrator pog{film, camera, sampler};
    std::cout << "Rendering... " << std::endl;
    Scene theScene;
    theScene.scene = scene;
    pog.Render(theScene);
    


}
