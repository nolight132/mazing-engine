#include <graphics/camera.h>
#include <graphics/draw.h>
#include <graphics/geometryBuffer.h>
#include <graphics/screen.h>
#include <main.h>
#include <map/map.h>
#include <memory/memoryManagement.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

const int size = 10;

int main()
{
    srand(time(NULL));

    int aabbCount = 0;
    AABB *aabbs = NULL;
    int **maze = generateMaze(size);
    initGeometry(&aabbs, &aabbCount, maze, size);

    Screen screen = {0};
    Camera camera = {0};
    initDraw();
    initScreen(&screen, COLS, LINES, 240);
    initCamera(&camera, 60, (Vector3){1, 10.0f, 7.5f}, (Vector3){0, 0, 1});

    double frameDuration = 1e9 / screen.fps;
    long long frameTime = 0;
    long long sleepTime = 0;

    while (gameRunning())
    { // Quit on 'q'
        clear();
        struct timespec start, end;
        // Record the start time of the frame
        clock_gettime(CLOCK_MONOTONIC, &start);
        deltaUpdate(&screen, &camera, aabbs, aabbCount, frameTime);
        // Calculate how long we need to sleep to maintain FPS
        clock_gettime(CLOCK_MONOTONIC, &end); // Get time again after operations
        frameTime = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
        sleepTime = (frameDuration - frameTime);
        if (sleepTime > 0)
        {
            struct timespec ts;
            ts.tv_sec = sleepTime / 1e9;       // Convert nanoseconds to seconds
            ts.tv_nsec = sleepTime % (int)1e9; // Remainder as nanoseconds
            nanosleep(&ts, NULL);              // Sleep for the remaining time
        }
        // Calculate and display FPS
        int currentFps = 1e9 / (frameTime + (sleepTime > 0 ? sleepTime : 0));
        float frameTimeF = (float)frameTime / 1e6;
        mvprintw(1, 0, "Res: %dx%d\n", screen.width, screen.height);
        mvprintw(4, 0, "frameTime: %.2f ms\n", frameTimeF);
        mvprintw(5, 0, "%d FPS", currentFps);
        refresh();
    }
    endwin();

    // Debug print
    debugPrintAABB(aabbs, aabbCount);
    printMap(maze, size);

    // Free memory
    free2DArray(maze, size);
    free(aabbs);

    return 0;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
// Update loop adjusted for delta time. Called every frame.
void deltaUpdate(Screen *screen, Camera *camera, AABB *aabbs, int aabbCount, double deltaTime)
{
    drawCall(*screen, *camera, aabbs, aabbCount);
}
#pragma GCC diagnostic pop

bool gameRunning()
{
    return getch() != 'q';
}

void debugPrintAABB(AABB *aabbs, int aabbCount)
{
    printf("Geometry data:\n");
    for (int i = 0; i < aabbCount; i++)
    {
        printf("AABB %d: (%f, %f, %f) - (%f, %f, %f)\n", i + 1, aabbs[i].min.x, aabbs[i].min.y, aabbs[i].min.z,
               aabbs[i].max.x, aabbs[i].max.y, aabbs[i].max.z);
    }
}