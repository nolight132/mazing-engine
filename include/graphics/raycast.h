#ifndef RAYCAST_H
#define RAYCAST_H

#include <types.h>
#include <stdbool.h>

bool rayIntersectsAABB(Vector3 rayOrigin, Vector3 rayDir, AABB box, float *t);

#endif // RAYCAST_H