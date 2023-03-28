#ifndef IMPORTOBJ_H
#define IMPORTOBJ_H

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
// Optional. define TINYOBJLOADER_USE_MAPBOX_EARCUT gives robust trinagulation. Requires C++11
#define TINYOBJLOADER_USE_MAPBOX_EARCUT
#define TINYOBJLOADER_USE_DOUBLE
#include "tiny_obj_loader.h"
#include "point.h"
#include <iostream>

namespace importOBJ{

    void loadOBJ(const std::string &file, std::unique_ptr<Point3f[]> &verts, std::unique_ptr<int[]> &vertexIndices, int *nVertices, int *nTriangles){
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

        
        *nVertices = attrib.vertices.size() / 3;
        *nTriangles = shapes[0].mesh.num_face_vertices.size();

        std::cout << "loading: " << *nVertices << " vertices" << std::endl;
        std::cout << "loading: " << *nTriangles << " triangles" << std::endl;

        verts.reset(new Point3f[*nVertices]);
        vertexIndices.reset(new int[*nTriangles * 3]);

        for(int i = 0; i < *nVertices; i++){
            verts[i] = Point3f(attrib.vertices[i * 3 + 0], attrib.vertices[i * 3 + 1], attrib.vertices[i * 3 + 2]);
        }

        for(int i = 0; i < *nTriangles; i++){
            vertexIndices[i*3] = shapes[0].mesh.indices[i*3].vertex_index;
            vertexIndices[i*3 + 1] = shapes[0].mesh.indices[i*3 + 1].vertex_index;
            vertexIndices[i*3 + 2] = shapes[0].mesh.indices[i*3 + 2].vertex_index;
        }


    }

};




#endif
