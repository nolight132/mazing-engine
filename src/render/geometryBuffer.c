#include "../types.h"
#include <stdlib.h>

int aabbCount = 0;
AABB *aabbs;

void generateAABBs(int **maze, int size, AABB *aabbs, int *aabbCount)
{
    *aabbCount = 0;
    for (int z = 0; z < size; z++)
    {
        for (int x = 0; x < size; x++)
        {
            if (maze[z][x] == WALL)
            {
                aabbs = (AABB *)realloc(aabbs, sizeof(AABB) * (*aabbCount + 1));
                aabbs[*aabbCount].min = (Vector3D){x, 0, z};
                aabbs[*aabbCount].max = (Vector3D){(x + 1), 1, (z + 1)};
                (*aabbCount)++;
            }
        }
    }
}