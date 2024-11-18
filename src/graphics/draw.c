#include <graphics/camera.h>
#include <graphics/screen.h>
#include <ncurses.h>
#include <stdio.h>
#include <time.h>

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
    initCamera(camera, 90, (Vector3D){0.5, 0.5, 0.5}, (Rotation){0, 0});
}

void drawCall(Screen screen, Camera camera)
{
    double frameDuration = 1000000000.0 / screen.fps;
    struct timespec start, end;
    long long frameTime = 0;
    long long sleepTime = 0;

    // Record the start time of the frame
    clock_gettime(CLOCK_MONOTONIC, &start); // Get current time (monotonic clock)
    clear();
    // Draw logic goes here
    mvprintw(0, 0, "Press 'q' to quit.");
    mvprintw(9, 0, "Screen FPS: %d", screen.fps);
    mvprintw(10, 0, "Camera FOV: %d", camera.fov);

    // Calculate how long we need to sleep to maintain FPS
    clock_gettime(CLOCK_MONOTONIC, &end); // Get time again after operations

    frameTime = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
    sleepTime = (frameDuration - frameTime); // Convert to microseconds

    if (sleepTime > 0)
    {
        struct timespec ts;
        ts.tv_sec = sleepTime / 1000000000;  // Convert nanoseconds to seconds
        ts.tv_nsec = sleepTime % 1000000000; // Remainder as nanoseconds
        nanosleep(&ts, NULL);                // Sleep for the remaining time
    }

    int currentFps = 1000000000 / (frameTime);
    float frameTimeF = (float)frameTime / 1000000.0;
    float sleepTimeF = (float)sleepTime / 1000000.0;
    mvprintw(1, 0, "sleepTime: %f\n", sleepTimeF);
    mvprintw(2, 0, "frameTime: %f\n", frameTimeF);
    mvprintw(3, 0, "%d FPS", currentFps);
    refresh();
}