#ifndef RAYCAST_H
#define RAYCAST_H

#include <graphics/camera.h>
#include <graphics/screen.h>
#include <stdbool.h>
#include <types.h>

float raycast(Ray ray, AABB box);
float renderCall(GeometryData data, Camera camera, Screen screen, int pixelRow, int pixelCol);

#endif // RAYCAST_H