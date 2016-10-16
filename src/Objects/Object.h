#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "../Ray.h"

//Base class for objects in a scene
class Object {
public:
    //How much light does the object reflect
    //In real world this is usually about 18%
    Vec3<float> albedo; 
    Object() 
    {
        this->albedo = Vec3<float>(0.18);
    }
    virtual ~Object() {}    
    //Check if a ray intersects the object
    virtual bool intersect(const Ray &, float &, int &, Vec2<float> &) const = 0;
    //Get data of the ray hit point
    virtual void get_surface_data(const Vec3<float> &, 
                                  const Vec3<float> &, 
                                  const int &,
                                  const Vec2<float> &, 
                                  Vec3<float> &, 
                                  Vec2<float> &) const = 0; 
};

#endif