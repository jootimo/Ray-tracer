
#include <fstream>
#include <chrono>
#include <random>
#include "Math.h"
#include "Ray.h"
#include "Objects/Object.h"
#include "Objects/Sphere.h"
#include "Objects/Triangle.h"
#include "Objects/TriangleMesh.h"
#include "Lights/DistantLight.h"
#include "Lights/PointLight.h"
#include "Atomics.h"

const float infinity = std::numeric_limits<float>::max();
std::atomic<uint32_t> num_ray_triangle_tests;
std::atomic<uint32_t> num_ray_triangle_isects;
std::atomic<uint32_t> num_primary_rays;
std::atomic<uint32_t> num_total_triangles;

struct Options {
    int img_width;
    int img_height;
    float fov; //field of view
    Matrix44<float> camera_to_world;
    Vec3<float> background_color;
    float shadow_bias; //a small value that is used to eliminate shadow acne
};

struct Intersect_info {
    const Object *hit_object = nullptr;
    float t_near = infinity;    //distance between ray origin and hit point
    Vec2<float> uv;             //texture coordinates
    int index = 0;
};

inline Vec3<float> mix(const Vec3<float> &a, const Vec3<float> &b, const float &mixValue)
{ return a * (1 - mixValue) + b * mixValue; }

inline float clamp(const float &lo, const float &hi, const float &v)
{ return std::max(lo, std::min(hi, v)); }

inline float deg_to_rad(const float &deg)
{ return deg * M_PI / 180; }
 

//return true if and object is hit
bool trace(const Ray &ray, 
           const std::vector<std::unique_ptr<Object>> &objects,
           Intersect_info &info)
{
    info.hit_object = nullptr;
    for(uint8_t i = 0; i < objects.size(); ++i) {
        float t_near_triangle = infinity;
        int index_triangle;
        Vec2<float> uv_triangle;


        if(objects[i]->intersect(ray, t_near_triangle, index_triangle, uv_triangle)
                && t_near_triangle < info.t_near) {

            //if an intersection is found and it is closer than the closest intersection
            info.hit_object = objects[i].get();
            info.t_near = t_near_triangle;
            info.index = index_triangle;
            info.uv = uv_triangle;
            
 
        }
    }
    return (info.hit_object != nullptr); 
}


Vec3<float> cast_ray(const Ray &ray,
                     const std::vector<std::unique_ptr<Object>> &objects,
                     const std::vector<std::unique_ptr<Light>> &lights,
                     const Options &options)
{
    Vec3<float> hit_color = Vec3<float>(0);
    Intersect_info info;

    //if the ray intersects an object, set the pixels color to the color
    //of the closest intersection point
    if(trace(ray, objects, info)) {
        Vec3<float> hit_point = ray.origin + ray.direction * info.t_near;
        Vec3<float> hit_normal;
        Vec2<float> hit_tex_coord;
        info.hit_object->get_surface_data(hit_point, ray.direction, info.index, info.uv, hit_normal, hit_tex_coord);
        
        //sum up the illumination of each light
        for(uint8_t i = 0; i < lights.size(); ++i) {
            Vec3<float> light_dir, light_intensity;
            Intersect_info shad_info;
            lights[i]->illuminate(hit_point, light_dir, light_intensity, shad_info.t_near);
            
            //Construct and cast shadow ray
            Ray shadow_ray(hit_point + hit_normal / options.shadow_bias, -light_dir, RayType::secondary);
            bool visible = !trace(shadow_ray, objects, shad_info);
            //if(visible) std::cout << "HIT" << std::endl;
            hit_color = hit_color + info.hit_object->albedo * 
                        (float) visible *  
                        (1 / M_PI) *
                        lights[i]->intensity * 
                        lights[i]->color *
                        std::max(0.f, hit_normal.dot_product(-light_dir));
        } 
    } else {
            hit_color = options.background_color;
    }

    return hit_color;
}



void render(const Options &options,
            const std::vector<std::unique_ptr<Object>> &objects,
            const std::vector<std::unique_ptr<Light>> &lights)
{
    std::cout << "Rendering..." << std::endl; 
    float aspect_ratio = options.img_width / (float) options.img_height;
    Vec3<float> *framebuffer = new Vec3<float>[options.img_width * options.img_height];
    Vec3<float> *pixels = framebuffer;
    Vec3<float> orig;
    orig = options.camera_to_world.multVecMatrix(Vec3<float>());

    for(int j = 0; j < options.img_height; ++j) {
        for(int i = 0; i < options.img_width; ++i) {
            //Normalized Device Coordinates in range [0, 1]
            float pixelNDC_x = (i + 0.5) / (float)options.img_width;
            float pixelNDC_y = (j + 0.5) / (float)options.img_height;
            //To screen space (range [-1, 1])
            float pixelSCR_x = 2 * pixelNDC_x - 1;
            float pixelSCR_y = 1 - (2 * pixelNDC_y);
            //To camera space (x axis to range [-aspect ratio, aspect ratio]) 
            //and account for field of view
            float pixelCAM_x = pixelSCR_x * aspect_ratio * tan(deg_to_rad(options.fov * 0.5));
            float pixelCAM_y = pixelSCR_y * tan(deg_to_rad(options.fov * 0.5));

            //Construct and cast ray for each pixel in frame
            std::atomic_fetch_add(&num_primary_rays, (uint32_t) 1);
            Ray ray;
            ray.origin = orig;
            ray.direction = options.camera_to_world.multDirMatrix(Vec3<float>(pixelCAM_x, pixelCAM_y, -1)); //z = -1 means forward
            ray.direction.normalize();            
            *(framebuffer++) = cast_ray(ray, objects, lights, options);
        }
    }


    //Output into a .ppm file
    std::ofstream ofs("./out1.ppm", std::ios::out | std::ios::binary);
    ofs << "P6\n" << options.img_width << " " << options.img_height << "\n255\n";
    
    for (int i = 0; i < options.img_height * options.img_width; ++i) {
        char r = (char)(255 * clamp(0, 1, pixels[i].x));
        char g = (char)(255 * clamp(0, 1, pixels[i].y));
        char b = (char)(255 * clamp(0, 1, pixels[i].z));
        ofs << r << g << b;
    }
    ofs.close();
}

int main(int argc, char **argv)
{
    Options options;
    options.img_width = 1920;
    options.img_height = 1080;
    options.fov = 51.52;
    options.camera_to_world = Matrix44<float>(0.945519, 0, -0.325569, 0, -0.179534, 0.834209, -0.521403, 0, 0.271593, 0.551447, 0.78876, 0, 4.208271, 8.374532, 17.932925, 1);
    options.background_color = Vec3<float>(0.2, 0.6, 0.8);
    options.shadow_bias = 0.001;

    //initialize random generator
    std::random_device rnd;
    std::mt19937 gen(rnd());
    std::uniform_real_distribution<> dis(0,1);
    
    gen.seed(0);
    
    //generate Scene
    //unique_ptr is automatically deleted when it goes out of scope
    std::vector<std::unique_ptr<Object>> objects;
    std::vector<std::unique_ptr<Light>> lights;

    TriangleMesh *mesh = generate_sphere_mesh(4, 6);
    //Sphere *s1 = new Sphere(6, Vec3<float>(-8, -2, -8), Vec3<float>(0.2, 0.8, 0.5));
    //Sphere *s2 = new Sphere(3, Vec3<float>(-8, -2, 6), Vec3<float>(0.2, 0.8, 0.5));
    //Sphere *s3 = new Sphere(2, Vec3<float>(2, 2, 6), Vec3<float>(0.2, 0.8, 0.5));
    objects.push_back(std::unique_ptr<Object>(mesh));
    //objects.push_back(std::unique_ptr<Object>(s1));
    //objects.push_back(std::unique_ptr<Object>(s2));
    //objects.push_back(std::unique_ptr<Object>(s3));

    Matrix44<float> l_to_w1;
    l_to_w1[3][0] = 2;
    l_to_w1[3][1] = 4;

    Matrix44<float> l_to_w2;
    l_to_w2[3][0] = 12;
    l_to_w2[3][1] = 14;
    l_to_w2[3][2] = -21;
    //l_to_w2 = Matrix44<float>(0.95292, 0.289503, 0.0901785, 0, -0.0960954, 0.5704, -0.815727, 0, -0.287593, 0.768656, 0.571365, 0, 0, 0, 0, 1);   
            //0.95292, 0.289503, 0.0901785, 0, -0.0960954, 0.5704, -0.815727, 0, -0.287593, 0.768656, 0.571365, 0, 0, 0, 0, 1); 
            //11.146836, -5.781569, -0.0605886, 0, -1.902827, -3.543982, -11.895445, 0, 5.459804, 10.568624, -4.02205, 0, 0, 0, 0, 1);     
    DistantLight *dl = new DistantLight(l_to_w1, Vec3<float>(0.6, 0.6, 1), 6);
    PointLight *pl = new PointLight(l_to_w2, Vec3<float>(1, 0.6, 0.6), 16); 

    lights.push_back(std::unique_ptr<Light>(pl));        
    lights.push_back(std::unique_ptr<Light>(dl));
    

    int num_spheres = 6;
    for(int i = 0; i < num_spheres; i++) {
        Vec3<float> random_pos((0.5 - dis(gen)) * 30, (0.5 - dis(gen)) * 30, (0.5 - dis(gen)) * 30);
        float random_rad = ((0.5 - dis(gen)) * 5);
        Vec3<float> color((float)(dis(gen)), (float)(dis(gen)), (float)(dis(gen)));
        objects.push_back(std::unique_ptr<Object>(new Sphere(random_rad, random_pos, color)));
    }
    
    //Measure rendering time
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    render(options, objects, lights);

    end = std::chrono::system_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Triangles: " << num_total_triangles << std::endl;
    std::cout << "Primary rays: " << num_primary_rays << std::endl;
    std::cout << "Ray-triangle tests: " << num_ray_triangle_tests << std::endl;
    std::cout << "Ray-triangle intersections: " << num_ray_triangle_isects << std::endl;
    std::cout << "Scene rendered in " << duration.count() << " seconds." << std::endl;

    return 0;
}
