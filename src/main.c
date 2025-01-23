#include <game/gameLogic.h>
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
#include <unistd.h>
#include <utils.h>

// Update loop adjusted for delta time. Called every frame.
void deltaUpdate(Screen *screen, Camera *camera, LevelData *levelData, int input, double deltaTime, double playTime)
{
    updateGeometry(&levelData->geometry, *camera);
    drawCall(*screen, *camera, levelData->geometry);
    handleInput(input, camera, levelData->geometry, deltaTime);
}

int main(int argc, char *argv[])
{
    int refreshRate = 70;
    int c;
    LevelData levelData = {0};
    levelData.size = 16;
    while ((c = getopt(argc, argv, "vuf:s:")) != -1)
    {
        switch (c)
        {
            case 'v':
                refreshRate = getRefreshRate();
                break;
            case 'u':
                refreshRate = 999999;
                break;
            case 'f':
                refreshRate = atoi(optarg);
                break;
            case 's':
                levelData.size = atoi(optarg);
                break;
            default:
                break;
        }
    }

    char *logFilePath = malloc(sizeof(char) * 512);
    char *latestLogFilePath = malloc(sizeof(char) * 512);
    FILE *logFile = createLog(logFilePath, latestLogFilePath);
    initLog(logFile);

    levelData.chunkSize = 4;

    if (levelData.size % levelData.chunkSize != 0)
        levelData.size += levelData.chunkSize - (levelData.size % levelData.chunkSize);

    levelData.mapData = generateMaze(levelData.size);
    levelData.startPos = levelData.mapData.start;
    levelData.score = 0;
    levelData.playTime = 0.0;
    levelData.startWorld = (Vector3){1.0f, levelData.startPos.x + 0.5f, levelData.startPos.y + 0.5f};

    initGeometry(&levelData.geometry, levelData.chunkSize, levelData.mapData.map, levelData.size);

    Screen screen = {0};
    Camera camera = {0};
    int renderDistance = 1; // Optimal value is 1, adjustable for debugging
    int fov = 50;
    initDraw();
    initScreen(&screen, COLS, LINES, refreshRate); // Reserve space for the debug window
    initCamera(&camera, fov, renderDistance, levelData.startWorld, (Rotation){0.0f, 0.0f});

    int debugHeight = 5;
    int debugWidth = COLS;
    int debugStartY = 0;
    int debugStartX = 0;
    WINDOW *debugWin = newwin(debugHeight, debugWidth, debugStartY, debugStartX);

    double frameDuration = 1e9 / (float)screen.refreshRate; // Target frame duration in nanoseconds
    long long sleepTime = 0;
    struct timespec start, end;

    // Timer for map printing
    long long mapPrintInterval = 200 * 1e6; // 200 milliseconds in nanoseconds
    long long lastMapPrintTime = 0;

    clock_gettime(CLOCK_MONOTONIC, &start);

    int ch;
    while ((ch = getch()) != 'q')
    { // Quit on 'q'
        // Record the end time of the frame
        clock_gettime(CLOCK_MONOTONIC, &end);

        // Calculate deltaTime in seconds
        long long frameTime = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
        double deltaTime = frameTime / 1e9; // Convert nanoseconds to seconds

        // Update total playtime
        levelData.playTime += deltaTime;

        // Call deltaUpdate with the correct deltaTime and levelData.playTime
        deltaUpdate(&screen, &camera, &levelData, ch, deltaTime, levelData.playTime);

        // Check if it's time to print the map
        long long currentTime = end.tv_sec * 1e9 + end.tv_nsec;
        if (currentTime - lastMapPrintTime >= mapPrintInterval)
        {
            printMap(levelData.mapData.map, levelData.size, toVector2(camera.position));
            lastMapPrintTime = currentTime; // Reset the timer
        }

        // Calculate how long we need to sleep to maintain FPS
        sleepTime = frameDuration - frameTime;
        if (sleepTime > 0)
        {
            struct timespec sleep;
            sleep.tv_sec = sleepTime / 1e9;       // Convert nanoseconds to seconds
            sleep.tv_nsec = sleepTime % (int)1e9; // Remainder as nanoseconds
            nanosleep(&sleep, NULL);              // Sleep for the remaining time
        }

        // Update start time for the next frame
        start = end;

        drawDebugInfo(debugWin, screen, camera, levelData, frameTime, sleepTime);
        if (checkForWin(toVector2(camera.position), levelData.mapData.goal))
        {
            levelData.score = 1000 / levelData.playTime;
            while (true)
            {
                victoryScreen(levelData.score, levelData.playTime);
            }
        }
        refresh();
    }
    endwin();

    rename(latestLogFilePath, logFilePath);
    fclose(logFile);

    // Free allocated memory
    free(logFilePath);
    free(latestLogFilePath);

    // Print total playtime when the game ends
    printf("Total playtime: %.2f seconds\n", levelData.playTime);

    return 0;
}
