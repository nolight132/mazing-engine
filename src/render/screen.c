#include <ncurses.h>
#include <time.h>
#include <unistd.h>

typedef struct Screen
{
    int width;
    int height;
    int fps;

    void (*init)(struct Screen *, int, int, int);
} Screen;

void initScreen(Screen *screen, int width, int height, int fps)
{
    screen->width = width;
    screen->height = height;
    screen->fps = fps;
}

void initDraw(Screen *screen)
{
    // Initialize ncurses
    initscr();
    cbreak();              // Disable line buffering
    noecho();              // Disable echoing of typed characters
    keypad(stdscr, TRUE);  // Enable special keys
    curs_set(0);           // Hide the default cursor
    nodelay(stdscr, TRUE); // Make getch non-blocking

    initScreen(screen, COLS, LINES, 75);
}

void draw(Screen screen)
{
    double frameDuration = 1000000000.0 / screen.fps;
    struct timespec start, end;
    long long frameTime = 0;
    long long sleepTime = 0;

    // Record the start time of the frame
    clock_gettime(CLOCK_MONOTONIC, &start); // Get current time (monotonic clock)

    clear();
    mvprintw(0, 0, "Press 'q' to quit.");

    // Calculate how long we need to sleep to maintain 60 FPS
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

    int currentFps = 1000000000 / (frameTime + sleepTime);
    float frameTimeF = (float)frameTime / 1000000.0;
    float sleepTimeF = (float)sleepTime / 1000000.0;
    mvprintw(1, 0, "sleepTime: %f\n", sleepTimeF);
    mvprintw(2, 0, "frameTime: %f\n", frameTimeF);
    mvprintw(3, 0, "%d FPS", currentFps);
    refresh();
}