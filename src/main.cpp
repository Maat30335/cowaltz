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


#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
// Optional. define TINYOBJLOADER_USE_MAPBOX_EARCUT gives robust trinagulation. Requires C++11
#define TINYOBJLOADER_USE_MAPBOX_EARCUT
#define TINYOBJLOADER_USE_DOUBLE
#include "tiny_obj_loader.h"






int main(){

    std::string inputfile = "stuff/cow.obj";
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "./"; // Path to material files

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(inputfile, reader_config)) {
    if (!reader.Error().empty()) {
        std::cerr << "TinyObjReader: " << reader.Error();
    }
    exit(1);
    }

    if (!reader.Warning().empty()) {
    std::cout << "TinyObjReader: " << reader.Warning();
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    
    int nVertices = attrib.vertices.size() / 3;
    int nTriangles = shapes[0].mesh.num_face_vertices.size();

    std::cout << "loading: " << nVertices << " vertices" << std::endl;
    std::cout << "loading: " << nTriangles << " triangles" << std::endl;

    std::unique_ptr<Point3f[]> verts(new Point3f[nVertices]);
    std::unique_ptr<Normal3f[]> normals(new Normal3f[nVertices]);
    std::unique_ptr<int[]> vertexIndices(new int[nTriangles * 3]);

    for(int i = 0; i < nVertices; i++){
        verts[i] = Point3f(attrib.vertices[i * 3 + 0], attrib.vertices[i * 3 + 1], attrib.vertices[i * 3 + 2]);
        std::cout << i << " : [" << attrib.vertices[i * 3 + 0] << ", " << attrib.vertices[i * 3 + 1] << ", " << attrib.vertices[i * 3 + 2] << "]" << std::endl;
        // normals[i] = Normal3f(attrib.normals[i * 3 + 0], attrib.normals[i * 3 + 1], attrib.normals[i * 3 + 2]);
    }

    std::cout << std::endl << std::endl;

    for(int i = 0; i < nTriangles; i++){
        vertexIndices[i*3] = shapes[0].mesh.indices[i*3].vertex_index;
        vertexIndices[i*3 + 1] = shapes[0].mesh.indices[i*3 + 1].vertex_index;
        vertexIndices[i*3 + 2] = shapes[0].mesh.indices[i*3 + 2].vertex_index;

        std::cout << i << " : vertex indices : " << std::endl;
        std::cout << vertexIndices[i*3 + 0] << std::endl;
        std::cout << vertexIndices[i*3 + 1] << std::endl;
        std::cout << vertexIndices[i*3 + 2] << std::endl;
    }

    std::cout << std::endl << std::endl;


    Transform SpherePos = Translate(Vector3f(0, 2, -3)) * Scale(0.3);
    Transform SpherePosInv = Inv(SpherePos);

    /*
    int nVertices = 3;
    int nTriangles = 1;

    std::unique_ptr<Point3f[]> verts(new Point3f[nVertices]);
    std::unique_ptr<int[]> vertexIndices(new int[nVertices]);

    verts[0] = Point3f(-1, 0, 0);
    verts[1] = Point3f(1, 0, 0);
    verts[2] = Point3f(0, 1, 0);
    vertexIndices[0] = 0;
    vertexIndices[1] = 1;
    vertexIndices[2] = 2;

    */

    std::vector<std::shared_ptr<Shape>> triangles = CreateTriangleMesh(&SpherePos, &SpherePosInv, 
            nTriangles, vertexIndices.get(), nVertices, verts.get(), nullptr, nullptr);

    PrimitiveList scene;

    for(int i = 0; i < nTriangles; i++){
        std::shared_ptr<Primitive> tri = std::make_shared<GeoPrimitive>(triangles[i]);
        scene.addPrim(tri);
    }

    
    Transform SpherePos2 = Translate(Vector3f(0, -100, -3));
    Transform SpherePosInv2 = Inv(SpherePos2);
    Transform Identity;
    const Point2i resolution{256, 256};


    std::shared_ptr<Shape> sphere_shape = std::make_shared<Sphere>(&SpherePos, &SpherePosInv, 1.5);
    std::shared_ptr<Primitive> sphere_prim = std::make_shared<GeoPrimitive>(sphere_shape);

    std::shared_ptr<Shape> sphere2_shape = std::make_shared<Sphere>(&SpherePos2, &SpherePosInv2, 99.0);
    std::shared_ptr<Primitive> sphere2_prim = std::make_shared<GeoPrimitive>(sphere2_shape);

    // scene.addPrim(sphere_prim);
    scene.addPrim(sphere2_prim);

    std::shared_ptr<Film> film = std::make_shared<Film>();
    std::shared_ptr<Camera> camera = std::make_shared<PerspectiveCamera>(&Identity, resolution, 125);
    std::shared_ptr<PixelSampler> sampler = std::make_shared<StratifiedSampler>(25);
    std::cout << sampler->samplesPerPixel << std::endl;
    
    NormalIntegrator pog{film, camera, sampler};
    std::cout << "Rendering... " << std::endl;
    pog.Render(scene);
    


}
