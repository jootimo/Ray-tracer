#include "Ray.h"

Ray::Ray()
{
    origin = Vec3<float>(0, 0, 0);
    direction = Vec3<float>(0, 0, 0);
    type = RayType::primary;
}

Ray::Ray(const Vec3<float> &orig, const Vec3<float> &dir, RayType r_type)
{
    origin = orig;
    direction = dir;
    type = r_type;
    //if not normalized
/*    if(direction.length() != 1) {
        direction.normalize();
    }
*/
}
