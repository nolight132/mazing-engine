#include "../types.h"
#include <stdbool.h>

const char *gradient = "@%#*+=-:. "; // Darkest to lightest
const int gradientLength = 10;       // Number of gradient levels

bool rayIntersectsAABB(Vector3D rayOrigin, Vector3D rayDir, AABB box, float *t)
{
    float tMin = (box.min.x - rayOrigin.x) / rayDir.x;
    float tMax = (box.max.x - rayOrigin.x) / rayDir.x;

    if (tMin > tMax)
    {
        float temp = tMin;
        tMin = tMax;
        tMax = temp;
    }

    float tyMin = (box.min.y - rayOrigin.y) / rayDir.y;
    float tyMax = (box.max.y - rayOrigin.y) / rayDir.y;

    if (tyMin > tyMax)
    {
        float temp = tyMin;
        tyMin = tyMax;
        tyMax = temp;
    }

    if (tMin > tyMax || tyMin > tMax)
        return false;

    if (tyMin > tMin)
        tMin = tyMin;
    if (tyMax < tMax)
        tMax = tyMax;

    float tzMin = (box.min.z - rayOrigin.z) / rayDir.z;
    float tzMax = (box.max.z - rayOrigin.z) / rayDir.z;

    if (tzMin > tzMax)
    {
        float temp = tzMin;
        tzMin = tzMax;
        tzMax = temp;
    }

    if (tMin > tzMax || tzMin > tMax)
        return false;

    if (tzMin > tMin)
        tMin = tzMin;
    if (tzMax < tMax)
        tMax = tzMax;

    *t = tMin; // The intersection distance along the ray
    return true;
}