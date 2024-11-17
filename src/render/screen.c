#include <ncurses.h>
#include <sys/time.h>
#include <unistd.h>

void drawLoop()
{
    // Initialize ncurses
    initscr();
    cbreak();              // Disable line buffering
    noecho();              // Disable echoing of typed characters
    keypad(stdscr, TRUE);  // Enable special keys
    curs_set(0);           // Hide the default cursor
    nodelay(stdscr, TRUE); // Make getch non-blocking

    const int frameDuration = 1000000 / 60; // 16.67 ms per frame

    while (getch() != 'q')
    { // Quit on 'q'
        // Record the start time of the frame
        struct timeval start;
        gettimeofday(&start, NULL);

        clear();
        mvprintw(0, 0, "Press 'q' to quit.");
        refresh();

        // Calculate how long we need to sleep to maintain 60 FPS
        struct timeval end;
        gettimeofday(&end, NULL);

        int frameTime = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
        int sleepTime = frameDuration - frameTime;

        if (sleepTime > 0)
        {
            usleep(sleepTime); // Sleep to maintain the frame rate
        }
    }
    endwin();
}