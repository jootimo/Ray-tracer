#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "../Math.h"
#include "TriangleMesh.h"

class Sphere : public Object {
public:
    float radius, radius2; //radius and its square
    Vec3<float> center, color;
    
    Sphere(const float &radius, 
           const Vec3<float> &center, 
           const Vec3<float> &color);
    
    bool intersect(const Ray &ray, float &t, int &, Vec2<float> &) const;
    
    void get_surface_data(const Vec3<float> &Phit,
                          const Vec3<float> &, 
                          const int &,
                          const Vec2<float> &, 
                          Vec3<float> &normal, 
                          Vec2<float> &tex) const;
    
    
};

//Make a triangle mesh from a sphere. 
//More divisions mean a better looking sphere
TriangleMesh* generate_sphere_mesh(float rad, int divisions);

#endif