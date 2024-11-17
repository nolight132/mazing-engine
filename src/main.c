#include "graphics/geometryBuffer.c"
#include "graphics/screen.c"
#include "map/map.c"
#include "memoryManagement.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

void free2DArray(int **array, int rows);

int main()
{
    srand(time(NULL));
    int size = 33;
    int **maze = generateMaze(size);
    AABB *aabbs = generateAABBs(maze, size, &aabbCount);

    printf("Geometry data:\n");
    for (int i = 0; i < aabbCount; i++)
    {
        printf("AABB %d: (%f, %f, %f) - (%f, %f, %f)\n", i + 1, aabbs[i].min.x, aabbs[i].min.y, aabbs[i].min.z,
               aabbs[i].max.x, aabbs[i].max.y, aabbs[i].max.z);
    }
    Screen screen;
    initDraw(&screen);
    while (getch() != 'q')
    { // Quit on 'q'
        // Logic goes here
        drawCall(screen);
    }
    endwin();
    printf("Resolution: %dx%d\n", screen.width, screen.height);

    // Free memory
    free2DArray(maze, size);
    free(aabbs);
    return 0;
}