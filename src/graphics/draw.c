#include "types.h"
#include <graphics/camera.h>
#include <graphics/raycast.h>
#include <graphics/screen.h>
#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <ui/debug.h>

void initDraw()
{
    // Initialize ncurses
    logWrite("Initializing ncurses...\n");
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

char getGradientChar(float distance)
{
    const char *gradient = "#@&%*=+~-,. ";
    const int gradientLength = 11; // Number of gradient levels
    const int maxDistance = 4;     // Maximum distance to render

    int index = (int)((distance / maxDistance) * gradientLength);
    return gradient[index < 0                 ? gradientLength
                    : index >= gradientLength ? gradientLength - 1
                    : index == gradientLength ? gradientLength - 1
                                              : index];
}

void drawCall(Screen screen, Camera camera, GeometryData geometry)
{
    // Draw logic
    for (int y = 4; y < screen.height; y++)
    {
        for (int x = 0; x < screen.width; x++)
        {
            char c = getGradientChar(renderCall(geometry, camera, screen, y, x));
            mvaddch(y, x, c);
        }
    }
}
