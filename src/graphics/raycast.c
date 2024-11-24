#include <math.h>
#include <stdbool.h>
#include <types.h>

const char *gradient = "@%#*+=-:. "; // Darkest to lightest
const int gradientLength = 10;       // Number of gradient levels
const int maxDistance = 10;          // Maximum distance to render

char getAsciiForDistance(float distance)
{
    const char *gradient = " .:-=+*%@";
    int index = (int)((1.0f - (distance / maxDistance)) * gradientLength);
    return gradient[index < 0 ? 0 : index > gradientLength ? gradientLength : index];
}

float raycast(Ray ray, AABB box)
{
    float t1 = (box.min.x - ray.origin.x) / ray.direction.x;
    float t2 = (box.max.x - ray.origin.x) / ray.direction.x;
    float t3 = (box.min.y - ray.origin.y) / ray.direction.y;
    float t4 = (box.max.y - ray.origin.y) / ray.direction.y;
    float t5 = (box.min.z - ray.origin.z) / ray.direction.z;
    float t6 = (box.max.z - ray.origin.z) / ray.direction.z;

    float tmin = fmax(fmax(fmin(t1, t2), fmin(t3, t4)), fmin(t5, t6));
    float tmax = fmin(fmin(fmax(t1, t2), fmax(t3, t4)), fmax(t5, t6));

    // if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behing us
    if (tmax < 0)
    {
        return -1;
    }

    // if tmin > tmax, ray doesn't intersect AABB
    if (tmin > tmax)
    {
        return -1;
    }

    if (tmin < 0)
    {
        return tmax;
    }
    return tmin;
}