#ifndef _ATOMICS_H_
#define _ATOMICS_H_

#include <atomic>

    extern std::atomic<uint32_t> num_ray_triangle_tests;
    extern std::atomic<uint32_t> num_ray_triangle_isects;
    extern std::atomic<uint32_t> num_primary_rays;
    extern std::atomic<uint32_t> num_total_triangles;

#endif