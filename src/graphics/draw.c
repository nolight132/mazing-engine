#include <graphics/camera.h>
#include <graphics/screen.h>
#include <ncurses.h>
#include <stdio.h>

void initDraw(Screen *screen, Camera *camera)
{
    // Initialize ncurses
    printf("Initializing ncurses...\n");
    initscr();
    cbreak();              // Disable line buffering
    noecho();              // Disable echoing of typed characters
    keypad(stdscr, TRUE);  // Enable special keys
    curs_set(0);           // Hide the default cursor
    nodelay(stdscr, TRUE); // Make getch non-blocking

    initScreen(screen, COLS, LINES, 60);
    initCamera(camera, 60, (Vector3){0.5, 0.5, 0.5}, (Rotation){0, 0});
}

void drawCall(Screen screen, Camera camera)
{
    // Draw logic goes here
    mvprintw(0, 0, "Press 'q' to quit.");
    mvprintw(2, 0, "Screen FPS: %d", screen.fps);
    mvprintw(3, 0, "Camera FOV: %d", camera.fov);
}