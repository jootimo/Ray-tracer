#include <cmath>
#include <memory>
#include "Sphere.h"

//M_PI is undefined in Windows
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


Sphere::Sphere(const float &radius, const Vec3<float> &center, const Vec3<float> &color)
{
    this->radius = radius;
    this->radius2 = radius * radius;
    this->center = center;
    this->color = color;
}


bool Sphere::intersect(const Ray &ray, float &t, int &, Vec2<float> &) const
{
    float t0, t1; //If found, solutions will be stored here
    
    //A sphere is defined as x²+y²+z² = R², so a point on the surface can be 
    //expressed as P² - R² = 0.
    //Any point on a ray is defined as O + tD (origin + t * direction)
    //So P² - R² = |O + tD|² - R² = 0
    //<=> O² + (Dt)² + 2ODt - R² 
    // => a = D², b = 2OD, c = O² - R²  
    Vec3<float> L(ray.origin - center);
    float a = ray.direction.dot_product(ray.direction);
    float b = 2 * ray.direction.dot_product(L);
    float c = L.dot_product(L) - radius2;
    if(!solve_quadratic(a, b, c, t0, t1)) {
        //no solutions found
        return false;
    }
    
    if(t0 < 0) {
        //intersection is behind camera
        if(t0 < 0) {
            //sphere is completely behind camera
            return false;
        } else {
            t0 = t1;
        }
    }

    t = t0;

    return true;
}

void Sphere::get_surface_data(const Vec3<float> &Phit, 
                            const Vec3<float> &, 
                            const int &,
                            const Vec2<float> &, 
                            Vec3<float> &normal, 
                            Vec2<float> &tex) const
{
    normal = Phit - center;
    normal.normalize();
    //calculate the spherical coordinates of the ray hit point Phit
    tex.x = (1 + atan2(normal.z, normal.x) / M_PI) * 0.5;
    tex.y = acosf(normal.y) / M_PI;
}

TriangleMesh* generate_sphere_mesh(float rad, int divisions)
{
     //generate points                                                                                                                                                                                      
    uint32_t num_vertices = (divisions - 1) * divisions + 2;        //How many vertices are needed
    std::unique_ptr<Vec3<float> []> P(new Vec3<float>[num_vertices]);
    std::unique_ptr<Vec3<float> []> N(new Vec3<float>[num_vertices]);
    std::unique_ptr<Vec2<float> []> tex(new Vec2<float>[num_vertices]);
    
    float u = -M_PI/2;
    float v = -M_PI;
    float du = M_PI / divisions;
    float dv = 2 * M_PI / divisions;

    P[0] = N[0] = Vec3<float>(0, -rad, 0);
    int k = 1;
    //Get the positions of the vertices
    for(int i = 0; i < divisions - 1; i++) {
        u += du;
        v = -M_PI;
        for(int j = 0; j < divisions; j++) {
            float x = rad * cos(u) * cos(v);
            float y = rad * sin(u);
            float z = rad * cos(u) * sin(v);
            P[k] = N[k] = Vec3<float>(x, y, z);
            tex[k].x = u / M_PI + 0.5;
            tex[k].y = v * 0.5 / M_PI + 0.5;
            v+= dv;
            k++;
        }
    }
    P[k] = N[k] = Vec3<float>(0, rad, 0);

    int num_polys = divisions * divisions;
    std::unique_ptr<int []> face_index(new int[num_polys]);
    std::unique_ptr<int []> verts_index(new int[(6 + (divisions - 1) * 4) * divisions]);

    //Create connectivity lists face_index and verts_index.
    //How are vertices connected together
    int vid = 1, num_v = 0, l = 0;
    k = 0;
    for(int i = 0; i < divisions; i++) {
        for(int j = 0; j < divisions; j++) {
            if(i == 0) {
                face_index[k++] = 3;
                verts_index[l] = 0;
                verts_index[l + 1] = j + vid;
                if(j == (divisions - 1)) {
                    verts_index[l + 2] = vid;
                } else {
                    verts_index[l + 2] = j + vid + 1;
                }
                l += 3;

            } else if(i == (divisions - 1)) {
                face_index[k++] = 3;
                verts_index[l] = j + vid + 1 - divisions;
                verts_index[l + 1] = vid + 1;
                if(j == (divisions - 1)) {
                    verts_index[l + 2] = vid + 1 - divisions;
                } else {
                    verts_index[l + 2] = j + vid + 2 - divisions;
                }
                l += 3;

                } else {
                    face_index[k++] = 4;
                    verts_index[l] = j + vid + 1 - divisions;
                    verts_index[l + 1] = j + vid + 1;
                    if(j == (divisions - 1)) {
                        verts_index[l + 2] = vid + 1;
                        verts_index[l + 3] = vid + 1 - divisions;
                    } else {
                        verts_index[l + 2] = j + vid + 2;
                        verts_index[l + 3] = j + vid + 2 - divisions;
                    }
                    l += 4;
                }
                num_v++;
            }
            vid = num_v;
        }
    
        return new TriangleMesh(num_polys, face_index, verts_index, P, N, tex);

}