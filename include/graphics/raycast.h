#ifndef RAYCAST_H
#define RAYCAST_H

#include <types.h>
#include <stdbool.h>

bool rayIntersectsAABB(Vector3D rayOrigin, Vector3D rayDir, AABB box, float *t);

#endif // RAYCAST_H