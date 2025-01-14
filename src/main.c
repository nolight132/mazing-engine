#include <graphics/camera.h>
#include <graphics/draw.h>
#include <graphics/geometryBuffer.h>
#include <graphics/screen.h>
#include <graphics/vector.h>
#include <input/input.h>
#include <map/map.h>
#include <memory/memoryManagement.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <types.h>
#include <ui/debug.h>
#include <ui/levelInfo.h>
#include <unistd.h>

// Update loop adjusted for delta time. Called every frame.
void deltaUpdate(Screen *screen, Camera *camera, Map *mapData, GeometryData *geometry, int input, double deltaTime)
{
    updateGeometry(geometry, *camera);
    drawCall(*screen, *camera, *geometry);
    handleInput(input, camera, *geometry, deltaTime);
    updateUi(*screen, *camera, *mapData, deltaTime);
    Vector2 playerPos = toVector2(camera->position);
    printMap(mapData->map, mapData->size, playerPos);
}

// Not using #define here to enable the user
// to change the size of the maze later
int size = 16;

int main(int argc, char *argv[])
{
    int refreshRate = 70;
    int c;
    while ((c = getopt(argc, argv, "vu")) != -1)
    {
        switch (c)
        {
            case 'v':
                refreshRate = getRefreshRate();
                break;
            case 'u':
                refreshRate = 999999;
                break;
            default:
                break;
        }
    }

    char *logFilePath = malloc(sizeof(char) * 512);
    char *latestLogFilePath = malloc(sizeof(char) * 512);
    FILE *logFile = createLog(logFilePath, latestLogFilePath);
    initLog(logFile);

    srand(time(NULL));

    int chunkSize = 4;
    if (size % chunkSize != 0)
        size += chunkSize - (size % chunkSize);

    Map mapData = generateMaze(size);

    int **maze = mapData.map;
    Vector2 startPos = mapData.start;
    Vector3 startWorld = (Vector3){1.0f, startPos.x + 0.5f, startPos.y + 0.5f};

    GeometryData geometry = {0};
    initGeometry(&geometry, chunkSize, maze, size);
    free2DArray((void **)maze, size);

    Screen screen = {0};
    Camera camera = {0};
    int renderDistance = 1; // Optimal value is 1, adjustable for debugging
    int fov = 50;
    initDraw();
    initScreen(&screen, COLS, LINES, refreshRate);
    initCamera(&camera, fov, renderDistance, startWorld, (Rotation){0.0f, 0.0f});

    double frameDuration = 1e9 / (float)screen.refreshRate;
    long long frameTime = frameDuration;
    long long sleepTime = 0;
    struct timespec start, end;

    int ch;
    while ((ch = getch()) != 'q')
    { // Quit on 'q'
        // Record the start time of the frame
        clock_gettime(CLOCK_MONOTONIC, &start);
        // Divide by 1e9 to convert nanoseconds to seconds
        deltaUpdate(&screen, &camera, &mapData, &geometry, ch, frameTime / 1e9f);
        // Calculate how long we need to sleep to maintain FPS
        clock_gettime(CLOCK_MONOTONIC, &end); // Get time again after operations
        frameTime = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
        sleepTime = frameDuration - frameTime;
        if (sleepTime > 0)
        {
            struct timespec sleep;
            sleep.tv_sec = sleepTime / 1e9;       // Convert nanoseconds to seconds
            sleep.tv_nsec = sleepTime % (int)1e9; // Remainder as nanoseconds
            nanosleep(&sleep, NULL);              // Sleep for the remaining time
        }
        printDebugInfo(screen, camera, geometry, size, frameTime, sleepTime);
        refresh();
    }
    endwin();

    rename(latestLogFilePath, logFilePath);
    fclose(logFile);
    return 0;
}
