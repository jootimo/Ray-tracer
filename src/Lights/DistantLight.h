#pragma once

#include <limits>
#include "Light.h"

class DistantLight : public Light {
public:
        
    DistantLight(const Matrix44<float> l_to_w, 
                 const Vec3<float> &color,
                 const float &intensity) : Light(l_to_w, color, intensity)
    {
        //this->color = color;
        //this->intensity = intensity;
        direction = l_to_w.multDirMatrix(Vec3<float>(0, 0, -1)); //point the light along the negative z-axis
        direction.normalize();
    }
    void illuminate(const Vec3<float> &point, 
                    Vec3<float> &light_dir, 
                    Vec3<float> &light_intensity, 
                    float &distance) const
    {
        light_dir = direction;
        light_intensity = color * intensity;
        distance = std::numeric_limits<int>::max(); 
    }
};