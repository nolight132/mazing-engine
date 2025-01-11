#ifndef DRAW_H
#define DRAW_H

#include <graphics/camera.h>
#include <graphics/screen.h>

void initDraw();
void drawCall(Screen screen, Camera camera, GeometryData geometry);

#endif // DRAW_H