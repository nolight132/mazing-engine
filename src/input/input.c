#include <graphics/camera.h>
#include <ncurses.h>
#include <types.h>

void handleInput(char input, Camera *camera, double deltaTime)
{
    if (input != ERR)
    {
        if (input == 'w')
            camera->position.z += 2.0f * deltaTime;
        if (input == 's')
            camera->position.z -= 2.0f * deltaTime;
        if (input == 'a')
            camera->position.x += 2.0f * deltaTime;
        if (input == 'd')
            camera->position.x -= 2.0f * deltaTime;
    }
}