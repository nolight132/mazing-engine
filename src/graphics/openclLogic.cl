#include "types.h"

__kernel void raycast_kernel(__global Ray *rays, __global AABB *boxes, __global float *results, int num_boxes)
{
    int id = get_global_id(0);
    Ray ray = rays[id];

    float tmin = INFINITY;

    // Iterate through the AABBs
    for (int i = 0; i < num_boxes; i++)
    {
        AABB box = boxes[i];

        float t1 = (box.min.x - ray.origin.x) / ray.direction.x;
        float t2 = (box.max.x - ray.origin.x) / ray.direction.x;
        float t3 = (box.min.y - ray.origin.y) / ray.direction.y;
        float t4 = (box.max.y - ray.origin.y) / ray.direction.y;
        float t5 = (box.min.z - ray.origin.z) / ray.direction.z;
        float t6 = (box.max.z - ray.origin.z) / ray.direction.z;

        float tmin_box = fmax(fmax(fmin(t1, t2), fmin(t3, t4)), fmin(t5, t6));
        float tmax_box = fmin(fmin(fmax(t1, t2), fmax(t3, t4)), fmax(t5, t6));

        if (tmax_box < 0)
        {
            results[id] = -1;
            return; // No intersection
        }

        if (tmin_box > tmax_box)
        {
            results[id] = -1;
            return; // No intersection
        }

        if (tmin_box < 0)
        {
            tmin_box = tmax_box;
        }

        if (tmin_box < tmin)
        {
            tmin = tmin_box;
        }
    }

    results[id] = tmin == INFINITY ? -1 : tmin;
}
