#pragma once

#include "../Math.h"

class Light {
public:
    Matrix44<float> light_to_world; //Transformation matrix
    Vec3<float> color;
    float intensity;
    Vec3<float> direction;          //distant lights
    Vec3<float> position;           //point lights

    Light(const Matrix44<float> l_to_w,
        const Vec3<float> &color,
        const float &intensity,
        Vec3<float>direction = Vec3<float>())
        : light_to_world{ l_to_w },
        color{ color },
        intensity{ intensity },
        direction{ direction } {}

    virtual ~Light() {}
    virtual void illuminate(const Vec3<float> &point, 
                    Vec3<float> &, 
                    Vec3<float> &, 
                    float &) const = 0;

};