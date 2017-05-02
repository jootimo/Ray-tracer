#include "Ray.h"

Ray::Ray()
    : origin{ Vec3<float>() },
    direction{ Vec3<float>() },
    type { RayType::primary } {}

Ray::Ray(const Vec3<float> &orig, const Vec3<float> &dir, RayType r_type)
    : origin{ orig }, direction{ dir }, type{ r_type }  {}
 
