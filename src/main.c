#include "map/map.c"
#include "memoryManagement.h"
#include "render/geometryBuffer.c"
#include "render/screen.c"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

void free2DArray(int **array, int rows);

int main()
{
    srand(time(NULL));
    int size = 17;
    int **maze = generateMaze(size);
    generateAABBs(maze, size, aabbs, &aabbCount);

    printf("AABBs: %d\n", aabbCount);
    printMap(maze, size);
    // for (int i = 0; i < aabbCount; i++)
    // {
    //     printf("AABB %d: (%f, %f, %f) - (%f, %f, %f)\n", i + 1, aabbs[i].min.x, aabbs[i].min.y, aabbs[i].min.z,
    //            aabbs[i].max.x, aabbs[i].max.y, aabbs[i].max.z);
    // }
    Screen screen;
    initDraw(&screen);
    while (getch() != 'q')
    { // Quit on 'q'
        draw(screen);
    }
    endwin();

    // Free memory
    free2DArray(maze, size);
    free(aabbs);
    return 0;
}