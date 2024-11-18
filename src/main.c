#include <graphics/camera.h>
#include <graphics/draw.h>
#include <graphics/geometryBuffer.h>
#include <graphics/screen.h>
#include <map/map.h>
#include <memory/memoryManagement.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

void debugPrintAABB(AABB *aabb, int aabbCount);

int main()
{
    srand(time(NULL));
    int size = 33;
    int aabbCount = 0;
    int **maze = generateMaze(size);
    AABB *aabbs = generateAABBs(maze, size, &aabbCount);

    Screen screen = {0};
    Camera camera = {0};
    initDraw(&screen, &camera);
    while (getch() != 'q')
    { // Quit on 'q'
        // Logic goes here
        drawCall(screen, camera);
    }
    endwin();

    // Debug print
    // debugPrintAABB(aabbs, aabbCount);
    // printf("Resolution: %dx%d\n", screen.width, screen.height);
    // printf("Resolution: %dx%d\n", COLS, LINES);
    // Free memory
    free2DArray(maze, size);
    free(aabbs);
    return 0;
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