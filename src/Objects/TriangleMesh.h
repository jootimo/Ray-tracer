#pragma once

#include <memory>
#include "../Math.h"
#include "Object.h"
#include "Triangle.h"

class TriangleMesh : public Object {
public:
    int num_triangles;                          //How many triangles in the mesh
    std::unique_ptr<Vec3<float> []> P;          //Triangles' vertex position
    std::unique_ptr<int []> triangles_index;    //Triangles' index array
    std::unique_ptr<Vec3<float> []> N;          //Triangle vertex normals
    std::unique_ptr<Vec2<float> []> tex_coord;  //Triangles' texture coordinates
    
    TriangleMesh(
        const int num_faces,                            //How many faces in the object
        const std::unique_ptr<int []> &face_index,      //How many vertices each face is made of
        const std::unique_ptr<int []> &verts_index,     //Which vertices make up which face
        const std::unique_ptr<Vec3<float> []> &verts,   //The vertices
        std::unique_ptr<Vec3<float> []> &normals,       //Normals of the vertices
        std::unique_ptr<Vec2<float> []> &tex);          
    
    //Return true if the ray intersects the triangle
    bool intersect(
        const Ray &ray,
        float &t_near,
        int &tri_index,
        Vec2<float> &uv) const;

    //Return surface data for this triangle
    void get_surface_data(
        const Vec3<float> &hit_point,
        const Vec3<float> &view_direction,
        const int &tri_index,
        const Vec2<float> &uv,
        Vec3<float> &hit_normal,
        Vec2<float> &hit_tex_coord) const;
    

};