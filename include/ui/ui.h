#ifndef UI_H
#define UI_H

#include <graphics/camera.h>
#include <graphics/screen.h>

void printDebugInfo(Screen screen, Camera camera, GeometryData geometry, int size, long long frameTime,
                    long long sleepTime);

#endif // UI_H