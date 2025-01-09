#include "graphics/camera.h"
#include "graphics/screen.h"
#include "graphics/vector.h"
#include "types.h"
#include <ncurses.h>
#include <stdio.h>

#pragma GCC diagnostic ignored "-Wunused-parameter"
void updateUi(Screen screen, Camera camera, Map mapData, double deltaTime)
{
    // Display distance to goal
    float distanceToGoal = distanceVector2(toVector2(camera.position), mapData.goal);
    mvprintw(3, 2, "Distance to goal: %.2f", distanceToGoal);
}
