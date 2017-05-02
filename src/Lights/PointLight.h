#pragma once

#define _USE_MATH_DEFINES //for M_PI in windows 
#include <math.h>
#include "Light.h"

class PointLight : public Light {
public:
    PointLight(const Matrix44<float> l_to_w, 
               const Vec3<float> &color,
               const float &intensity) : Light(l_to_w, color, intensity)
    {
        position = l_to_w.multDirMatrix(Vec3<float>(0)); 
    }

    void illuminate(const Vec3<float> &point, 
                          Vec3<float> &light_dir, 
                          Vec3<float> &light_intensity, 
                          float &distance) const
    {
        light_dir = point - position;
        //Calculate the distance's square needed for calculating the intensity
        float dist_sq = light_dir.x * light_dir.x +
                        light_dir.y * light_dir.y +
                        light_dir.z * light_dir.z;
        distance = sqrtf(dist_sq);
        //normalize light direction
        light_dir.x /= distance;
        light_dir.y /= distance;
        light_dir.z /= distance;

        //square falloff according to the inverse-square law
        //https://en.wikipedia.org/wiki/Inverse-square_law
        light_intensity = color * intensity / (4 * M_PI * dist_sq);
    }
};