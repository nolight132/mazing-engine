#ifndef DRAW_H
#define DRAW_H

#include <graphics/camera.h>
#include <graphics/screen.h>

void initDraw();
void drawCall(Screen screen, Camera camera, AABB **aabbs);

#endif // DRAW_H