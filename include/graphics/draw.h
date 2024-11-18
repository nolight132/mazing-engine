#ifndef DRAW_H
#define DRAW_H

#include <graphics/screen.h>
#include <graphics/camera.h>

void initDraw(Screen *screen, Camera *camera);
void drawCall(Screen screen, Camera camera);

#endif // DRAW_H