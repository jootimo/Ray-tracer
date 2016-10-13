#include <limits>
#include "TriangleMesh.h"
#include "Atomics.h"

const float infinity = std::numeric_limits<float>::max();

TriangleMesh::TriangleMesh(
        const int num_faces,
        const std::unique_ptr<int []> &face_index,
        const std::unique_ptr<int []> &verts_index,
        const std::unique_ptr<Vec3<float> []> &verts,
        std::unique_ptr<Vec3<float> []> &normals,
        std::unique_ptr<Vec2<float> []> &tex)
{
    num_triangles = 0;
    int k = 0;
    int max_vert_index = 0;

    //How many triangles need to be created?
    for(int i = 0; i < num_faces; ++i) {
        num_triangles += face_index[i] - 2;
        for(int j = 0; j < face_index[i]; ++j) {
            if(verts_index[k + j] > max_vert_index) {
                max_vert_index = verts_index[k + j];
            }      
        }
        k += face_index[i];
    }
    max_vert_index += 1;
    std::cout << "Generating mesh with " << num_triangles << " triangles." << std::endl;
    std::atomic_fetch_add(&num_total_triangles, (uint32_t) num_triangles);

    //Allocate memory for positions of the mesh vertices
    P = std::unique_ptr<Vec3<float> []>(new Vec3<float>[max_vert_index]);
    for(int i = 0; i < max_vert_index; ++i) {
        P[i] = verts[i];
    }

    //Allocate memory for triangle indices
    triangles_index = std::unique_ptr<int []>(new int[num_triangles * 3]);

    //Allocate memory for normals and texture coordinates
    N = std::unique_ptr<Vec3<float> []>(new Vec3<float>[num_triangles * 3]);
    tex_coord = std::unique_ptr<Vec2<float> []>(new Vec2<float>[num_triangles * 3]);

    //Set up the triangles' index, Normal and texture coordinates arrays
    //with each triangle's v0, v1 and v2 
    int l = 0;
    for(int i = 0, k = 0; i < num_faces; ++i) {         //for each face
       for(int j = 0; j < face_index[i] - 2; ++j) {     //for each triangle
            triangles_index[l] = verts_index[k];
            triangles_index[l + 1] = verts_index[k + j + 1];
            triangles_index[l + 2] = verts_index[k + j + 2];
            N[l] = normals[k];
            N[l + 1] = normals[k + j + 1];
            N[l + 2] = normals[k + j + 2];
            tex_coord[l] = tex[k];
            tex_coord[l + 1] = tex[k + j + 1];
            tex_coord[l + 2] = tex[k + j + 2];
            l += 3;
        }
        k += face_index[i];
    }
}

bool TriangleMesh::intersect(const Ray &ray,
                             float &t_near, 
                             int &tri_index, 
                             Vec2<float> &uv) const 
{
    int j = 0;
    bool intersect = false;

    Triangle tri(P[triangles_index[0]], P[triangles_index[1]], P[triangles_index[2]]);
    //Loop over all the triangles in the mesh and store the closest intersection point 
    
    for(int i = 0; i < num_triangles; ++i) {
        tri.v0 = P[triangles_index[j]];
        tri.v1 = P[triangles_index[j + 1]];
            tri.v2 = P[triangles_index[j + 2]];
        float t, u, v = infinity;
        if(tri.intersect(ray, t, u, v) && t < t_near) {
            t_near = t;     //the intersection distance
            uv.x = u;       //barycentric coordinates
            uv.y = v;
            tri_index = i;  //which triangle is hit
            intersect = true;
        }
    
        //next triangle
        j += 3;             
        
    }
    return intersect;
}

void TriangleMesh::get_surface_data(
        const Vec3<float> &hit_point,
        const Vec3<float> &view_direction,
        const int &tri_index,
        const Vec2<float> &uv,
        Vec3<float> &hit_normal,
        Vec2<float> &hit_tex_coord) const
{
    //texture coordinates 
    const Vec2<float> &st0 = tex_coord[tri_index * 3];
    const Vec2<float> &st1 = tex_coord[tri_index * 3 + 1];
    const Vec2<float> &st2 = tex_coord[tri_index * 3 + 2];
    hit_tex_coord = st0 * (1 - uv.x - uv.y) + st1 * uv.x + st2 * uv.y;

    /*
    //face normals:
    //will give a more crude look than with vertex normals 
    const Vec3<float> &v0 = P[triangles_index[tri_index * 3]];
    const Vec3<float> &v1 = P[triangles_index[tri_index * 3 + 1]];
    const Vec3<float> &v2 = P[triangles_index[tri_index * 3 + 2]];
    //The normal is the cross product of the vertices defining the face
    hit_normal = (v1 -v0).cross_product(v2 - v0);
    hit_normal.normalize();
    */
    
    //vertex normals
    //necessary for Gouraud shading aka. smooth shading
    //https://en.wikipedia.org/wiki/Gouraud_shading
    const Vec3<float> &n0 = P[triangles_index[tri_index * 3]];
    const Vec3<float> &n1 = P[triangles_index[tri_index * 3 + 1]];
    const Vec3<float> &n2 = P[triangles_index[tri_index * 3 + 2]];
    hit_normal = n0 * (1 - uv.x - uv.y) + n1 * uv.x + n2 * uv.y;
    hit_normal.normalize();
    
}