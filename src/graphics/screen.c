#ifndef SCREEN_H
#define SCREEN_H

#include <GLFW/glfw3.h>
#include <graphics/camera.h>
#include <stdio.h>

typedef struct Screen
{
    int width;
    int height;
    int fps;

    // Function pointers
    void (*init)(struct Screen *, int, int, int);
} Screen;

void initScreen(Screen *screen, int width, int height, int fps)
{
    printf("Initializing screen...\n");
    screen->width = width;
    screen->height = height;
    screen->fps = fps;
    screen->init = initScreen;
}

int getRefreshRate()
{
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    if (monitor == NULL)
    {
        fprintf(stderr, "Failed to get primary monitor\n");
        glfwTerminate();
        return -1;
    }

    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    if (mode == NULL)
    {
        fprintf(stderr, "Failed to get video mode\n");
        glfwTerminate();
        return -1;
    }
    return mode->refreshRate;

    // Terminate GLFW
    glfwTerminate();
}

#endif // SCREEN_H
