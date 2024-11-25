#include <graphics/camera.h>
#include <graphics/raycast.h>
#include <graphics/screen.h>
#include <ncurses.h>
#include <stdio.h>

extern const int size;

void initDraw()
{
    // Initialize ncurses
    printf("Initializing ncurses...\n");
    initscr();
    cbreak();              // Disable line buffering
    noecho();              // Disable echoing of typed characters
    keypad(stdscr, TRUE);  // Enable special keys
    curs_set(0);           // Hide the default cursor
    nodelay(stdscr, TRUE); // Make getch non-blocking
}

void drawCall(Screen screen, Camera camera)
{
    // Draw logic
    for (int y = 6; y < screen.height; y++)
    {
        for (int x = 0; x < screen.width; x++)
        {
            // char c = getGradientChar(screen, x, y);
            // mvaddch(y, x, c);
            mvaddch(y, x, ':');
        }
    }
    mvprintw(0, 0, "Press 'q' to quit.");
    mvprintw(2, 0, "Screen FPS: %d", screen.fps);
    mvprintw(3, 0, "Camera FOV: %d", camera.fov);
}