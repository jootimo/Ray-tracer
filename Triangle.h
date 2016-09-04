#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "Math.h"
#include "Ray.h"

class Triangle {
public:
    Vec3<float> v0, v1, v2; //Vertices
    Triangle();
    Triangle(const Vec3<float> &a, const Vec3<float> &b, const Vec3<float> &c);
    //Test for ray-triangle intersection.
    //if found, the barycentric coordinates are stored in t, u and v. 
    bool intersect(const Ray &ray, float &t, float &u, float &v);
};

#endif