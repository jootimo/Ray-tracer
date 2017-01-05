//test model from http://tf3dm.com/3d-model/joseph-stalin-60168.html

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include "ObjLoader.h"

TriangleMesh load_obj(const std::string filename) {
    std::ifstream stream;
    stream.open(filename);
    if (!stream.is_open()) {
        std::cerr << "Could not open obj-file" << std::endl;
        exit(EXIT_FAILURE);
    } 

    int num_faces = 0;
    int num_vertices = 0;
    
    std::vector<Vec3<float>> vertices;
    std::vector<int> verts_indices;
    std::string line;
    
    while(getline(stream, line)) {
        
        std::istringstream iss(line);
        std::string element;
        
        (iss >> element);
        
        //v for vertex
        if(element == "v") {
            num_vertices++;

            double p, q, r;
            iss >> p >> q >> r;
            
            vertices.push_back(Vec3<float>(p, q, r));
        } 
        //f for face
        else if (element == "f") {
            num_faces++;

            int x, y, z;
            iss >> x >> y >> z; 

            verts_indices.push_back(x);
            verts_indices.push_back(y);
            verts_indices.push_back(z);
        }

    }

    stream.close();

    //Vectors to arrays for the TriangleMesh constructor, clumsy
    std::unique_ptr<int []> verts_in_face_arr(new int[num_faces]);
    std::unique_ptr<int []> verts_indices_arr(new int[num_faces]);
    std::unique_ptr<Vec3<float> []> vertices_arr(new Vec3<float>[num_vertices]);

    for(int i = 0, j = 0; (i < num_faces), (j < num_vertices); i++, j++) {
        verts_indices_arr[i] = verts_indices[i];
        verts_in_face_arr[i] = 3;
        vertices_arr[j] = vertices.at(j);

    }
  
    //Texture and normal arrays needed by the TriangleMesh constructor
    std::unique_ptr<Vec3<float>[]> normals(new Vec3<float>[num_vertices]);
    std::unique_ptr<Vec2<float>[]> tex(new Vec2<float>[num_vertices]);

    return TriangleMesh(num_faces,
                        verts_in_face_arr,
                        verts_indices_arr,
                        vertices_arr,
                        normals,
                        tex);
    

}


