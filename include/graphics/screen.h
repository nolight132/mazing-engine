#ifndef SCREEN_H
#define SCREEN_H

typedef struct Screen
{
    int width;
    int height;
    int fps;

    void (*init)(struct Screen *, int, int, int);
} Screen;

void initScreen(Screen *screen, int width, int height, int fps);

#endif // SCREEN_H
