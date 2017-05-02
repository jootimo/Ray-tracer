#pragma once

#include "Math.h"

enum class RayType {primary, secondary};
class Ray {
public:
    Vec3<float> origin;
    Vec3<float> direction; //a normalized vector
    RayType type;
    Ray();
    Ray(const Vec3<float> &orig, const Vec3<float> &dir, RayType r_type);
};