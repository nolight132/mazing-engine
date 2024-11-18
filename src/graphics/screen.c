#ifndef SCREEN_H
#define SCREEN_H

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

#endif // SCREEN_H