#include "types.h"
#include <graphics/camera.h>
#include <graphics/raycast.h>
#include <graphics/screen.h>
#include <locale.h>
#include <ncurses.h>
#include <stdio.h>

void initDraw()
{
    // Initialize ncurses
    printf("Initializing ncurses...\n");
    setlocale(LC_ALL, "");
    initscr();
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    timeout(-1);
    printf("\033[?1003h\n"); // Enable mouse motion events in terminal
    fflush(stdout);
    cbreak();              // Disable line buffering
    noecho();              // Disable echoing of typed characters
    keypad(stdscr, TRUE);  // Enable special keys
    curs_set(0);           // Hide the default cursor
    nodelay(stdscr, TRUE); // Make getch non-blocking
}

char getGradientChar(float distance, int renderDistance)
{
    const char *gradient = "#@&%*=+~-,. ";
    const int gradientLength = 11;          // Number of gradient levels
    const int maxDistance = renderDistance; // Maximum distance to render

    int index = (int)((distance / maxDistance) * gradientLength);
    return gradient[index < 0                 ? gradientLength
                    : index >= gradientLength ? gradientLength - 1
                    : index == gradientLength ? gradientLength - 1
                                              : index];
}

void drawCall(Screen screen, Camera camera, GeometryData geometry)
{
    // Draw logic
    int renderDistanceBlocks = camera.renderDistance * geometry.defaultChunkSize;
    for (int y = 6; y < screen.height; y++)
    {
        for (int x = 0; x < screen.width; x++)
        {
            char c = getGradientChar(raycastCall(geometry, camera, screen, y, x), renderDistanceBlocks);
            mvaddch(y, x, c);
        }
    }
    mvprintw(0, 0, "Press 'q' to quit.");
    mvprintw(2, 0, "Screen FPS: %d", screen.fps);
    mvprintw(3, 0, "Camera FOV: %d", camera.fov);
}