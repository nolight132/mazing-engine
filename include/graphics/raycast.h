#ifndef RAYCAST_H
#define RAYCAST_H

#include <graphics/camera.h>
#include <graphics/screen.h>
#include <stdbool.h>
#include <types.h>

float raycastCall(AABB *aabbs, int aabbCount, Camera camera, Screen screen, int pixelRow, int pixelCol);

#endif // RAYCAST_H