#ifndef IMPORTOBJ_H
#define IMPORTOBJ_H

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
// Optional. define TINYOBJLOADER_USE_MAPBOX_EARCUT gives robust trinagulation. Requires C++11
#define TINYOBJLOADER_USE_MAPBOX_EARCUT
#define TINYOBJLOADER_USE_DOUBLE
#include "tiny_obj_loader.h"
#include "point.h"
#include "normal.h"
#include "triangle.h"
#include <iostream>

namespace importOBJ{

    // note, this only supports importing a single shape, i'm not sure how multiple shapes work right now
    // also, you need to split the mesh such that every vertex has a unique UV and Normal.
    void loadOBJ(const std::string &file, std::unique_ptr<Point3f[]> &verts, std::unique_ptr<Normal3f[]> &normals, std::unique_ptr<Point2f[]> &uvs, std::unique_ptr<int[]> &vertexIndices, int &nVertices, int &nTriangles){
        std::string inputfile = file;
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

        
        nVertices = attrib.vertices.size() / 3;
        nTriangles = shapes[0].mesh.num_face_vertices.size();

        std::cout << "loading: " << nVertices << " vertices" << std::endl;
        std::cout << "loading: " << nTriangles << " triangles" << std::endl;

        verts.reset(new Point3f[nVertices]);
        vertexIndices.reset(new int[nTriangles * 3]);

        for(int i = 0; i < nVertices; i++){
            verts[i] = Point3f(attrib.vertices[i * 3 + 0], attrib.vertices[i * 3 + 1], attrib.vertices[i * 3 + 2]);
        }

        for(int i = 0; i < nTriangles; i++){
            vertexIndices[i*3] = shapes[0].mesh.indices[i*3].vertex_index;
            vertexIndices[i*3 + 1] = shapes[0].mesh.indices[i*3 + 1].vertex_index;
            vertexIndices[i*3 + 2] = shapes[0].mesh.indices[i*3 + 2].vertex_index;
        }

        if(shapes[0].mesh.indices[0].normal_index >= 0){ // check if we have normals, note we assuming if there is one normal they are all there
            normals.reset(new Normal3f[nVertices]);
            for(int i = 0; i < nTriangles; i++){
                normals[vertexIndices[i * 3 + 0]] = Normal3f(attrib.normals[3*shapes[0].mesh.indices[i * 3 + 0].normal_index + 0],
                                                            attrib.normals[3*shapes[0].mesh.indices[i * 3 + 0].normal_index + 1],
                                                            attrib.normals[3*shapes[0].mesh.indices[i * 3 + 0].normal_index + 2]);

                normals[vertexIndices[i * 3 + 1]] = Normal3f(attrib.normals[3*shapes[0].mesh.indices[i * 3 + 1].normal_index + 0],
                                                            attrib.normals[3*shapes[0].mesh.indices[i * 3 + 1].normal_index + 1],
                                                            attrib.normals[3*shapes[0].mesh.indices[i * 3 + 1].normal_index + 2]);

                normals[vertexIndices[i * 3 + 2]] = Normal3f(attrib.normals[3*shapes[0].mesh.indices[i * 3 + 2].normal_index + 0],
                                                            attrib.normals[3*shapes[0].mesh.indices[i * 3 + 2].normal_index + 1],
                                                            attrib.normals[3*shapes[0].mesh.indices[i * 3 + 2].normal_index + 2]);
            }
        }else{
            normals.reset();
        }

        if(shapes[0].mesh.indices[0].texcoord_index >= 0){ // check if we have uvs, note we assuming if there is one uv they are all there
            uvs.reset(new Point2f[nVertices]);
            for(int i = 0; i < nTriangles; i++){
                uvs[vertexIndices[i*3]] = Point2f(attrib.texcoords[2*shapes[0].mesh.indices[i*3].texcoord_index + 0],
                                                attrib.texcoords[2*shapes[0].mesh.indices[i*3].texcoord_index + 1]);

                uvs[vertexIndices[i*3 + 1]] = Point2f(attrib.texcoords[2*shapes[0].mesh.indices[i*3 + 1].texcoord_index + 0],
                                                attrib.texcoords[2*shapes[0].mesh.indices[i*3 + 1].texcoord_index + 1]);

                uvs[vertexIndices[i*3 + 2]] = Point2f(attrib.texcoords[2*shapes[0].mesh.indices[i*3 + 2].texcoord_index + 0],
                                                attrib.texcoords[2*shapes[0].mesh.indices[i*3 + 2].texcoord_index + 1]);
            }
        }else{
            uvs.reset();
        }


    }

    void addOBJ(std::vector<std::shared_ptr<Primitive>> &mesh, const char* filename, Transform* transform, Transform* inverse,std::shared_ptr<Material> material){
        std::unique_ptr<Point3f[]> verts;
        std::unique_ptr<Normal3f[]> normals;
        std::unique_ptr<Point2f[]> uvs;
        std::unique_ptr<int[]> vertexIndices;
        int nVertices;
        int nTriangles;

        importOBJ::loadOBJ(filename, verts, normals, uvs, vertexIndices, nVertices, nTriangles);
        
        std::vector<std::shared_ptr<Shape>> triangles = CreateTriangleMesh(transform, inverse, 
                nTriangles, vertexIndices.get(), nVertices, verts.get(), normals.get(), uvs.get());

        for(int i = 0; i < nTriangles; i++){
            std::shared_ptr<Primitive> tri = std::make_shared<GeoPrimitive>(triangles[i], material);
            mesh.push_back(tri);
        }
    }

};




#endif
