#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#include "map/map.c"
#include "memoryManagement.h"
#include "render/geometryBuffer.c"
#include "render/screen.c"

void free2DArray(int **array, int rows);

int main()
{
    srand(time(NULL));
    int size = 17;
    int **maze = generateMaze(size);

    generateAABBs(maze, size, aabbs, &aabbCount);
    printMap(maze, size);

    // printf("Triangles (%d):\n", triangleCount);
    // for (int i = 0; i < triangleCount; i++)
    // {
    //     printf("  (%d, %d, %d)\n", triangles[i].a, triangles[i].b, triangles[i].c);
    // }

    drawLoop();

    // Free memory
    free2DArray(maze, size);
    return 0;
}