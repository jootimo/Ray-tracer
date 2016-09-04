#include "Triangle.h"

//Do we render the triangles' backsides?
const bool backface_culling = false;
//A very small value. Needed to check rounding errors
const float epsilon = 0.00000001; 

Triangle::Triangle() 
{
    v0 = v1 = v2 = Vec3<float>(0, 0, 0);
}

Triangle::Triangle(const Vec3<float> &a, const Vec3<float> &b, const Vec3<float> &c)
{
    v0 = a;
    v1 = b;
    v2 = c;
}

//The Möller-Trumbore algorithm for fast intersection detection
//https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
bool Triangle::intersect(const Ray &ray, float &t, float &u, float &v)
{
    //Vectors that determine the triangles edges
    Vec3<float> v0v1 = v1 - v0;
    Vec3<float> v0v2 = v2 - v0;

    //Needed for calculating the determinant and u  
    Vec3<float> pvec = ray.direction.cross_product(v0v2);
    //Determinant can be calculated as a scalar triple product
    float determinant = v0v1.dot_product(pvec);

    if(backface_culling && determinant < epsilon) {
        //If determinant is negative, it is facing away from the camera
        return false;
    } else if(determinant < epsilon && determinant > -epsilon) {
        //If it is close to 0, the ray and triangle are at a 90 degree angle
        //and the triangle is not visible
        return false;
    }

    float inverse_det = 1 / determinant;

    Vec3<float> tvec = ray.origin - v0; //Distance from ray origin to v0
    u = tvec.dot_product(pvec) * inverse_det;
    if (u < 0 || u > 1) {
        //The ray misses the triangle;
        return false;
    }

    Vec3<float> qvec = tvec.cross_product(v0v1);
    v = ray.direction.dot_product(qvec) * inverse_det;
    if(v < 0 || u + v > 1) {
        //Miss!
        return false;
    }

    t = v0v2.dot_product(qvec) * inverse_det;

    return true;
}