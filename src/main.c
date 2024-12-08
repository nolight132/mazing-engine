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
#include <time.h>
#include <types.h>
#include <unistd.h>

// Update loop adjusted for delta time. Called every frame.
void deltaUpdate(Screen *screen, Camera *camera, GeometryData *geometry, int input, double deltaTime)
{
    updateGeometry(geometry, *camera);
    drawCall(*screen, *camera, *geometry);
    handleInput(input, camera, *geometry, deltaTime);
}

// Not using #define here to enable the user
// to change the size of the maze later
const int size = 128;

int main()
{
    srand(time(NULL));

    int chunkSize = 4;
    int **maze = generateMaze(size);
    // Debug print
    printMap(maze, size);
    GeometryData geometry = {0};
    initGeometry(&geometry, chunkSize, maze, size);
    free2DArray((void **)maze, size);

    Screen screen = {0};
    Camera camera = {0};
    int renderDistance = 1; // Optimal value is 1, adjustable for debugging
    int fov = 50;
    initDraw();
    initScreen(&screen, COLS, LINES, 60);
    initCamera(&camera, fov, renderDistance, (Vector3){1.0f, 2.5f, 2.5f}, (Rotation){0.0f, 0.0f});

    double frameDuration = 1e9 / (float)screen.fps;
    long long frameTime = frameDuration;
    long long sleepTime = 0;

    int ch;
    while ((ch = getch()) != 'q')
    { // Quit on 'q'
        clear();
        struct timespec start, end;
        // Record the start time of the frame
        clock_gettime(CLOCK_MONOTONIC, &start);
        // Divide by 1e9 to convert nanoseconds to seconds
        deltaUpdate(&screen, &camera, &geometry, ch, frameTime / 1e9f);
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
        // Print debug info
        mvprintw(0, 0, "Press 'q' to quit.");
        mvprintw(1, 0, "| Res: %dx%d", screen.width, screen.height);
        mvprintw(1, 30, "| Map: %dx%d (%d chunks)", size, size, size * size);
        mvprintw(1, 60, "|");
        mvprintw(2, 0, "| YXZ: (%.2f, %.2f, %.2f)", camera.position.y, camera.position.x, camera.position.z);
        mvprintw(2, 30, "| Chunk: (%d, %d)", geometry.currentChunkZ, geometry.currentChunkX);
        mvprintw(2, 60, "| P/Y: %.2f %.2f", camera.rotation.pitch, camera.rotation.yaw);
        mvprintw(3, 0, "| %d FPS (%d max)", currentFps, screen.fps);
        mvprintw(3, 30, "| frameTime: %.2f ms", frameTimeF);
        mvprintw(3, 60, "|");
        refresh();
    }
    endwin();

    return 0;
}