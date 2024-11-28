#include <stdio.h>
#include <stdlib.h>
#include <types.h>

AABB *generateAABBs(int **maze, int size, int *aabbCount)
{
    AABB *aabbs = NULL;
    *aabbCount = 0;
    for (int z = 0; z < size; z++)
    {
        for (int x = 0; x < size; x++)
        {
            if (maze[z][x] == WALL)
            {
                aabbs = (AABB *)realloc(aabbs, sizeof(AABB) * (*aabbCount + 1));
                aabbs[*aabbCount].min = (Vector3){0, x, z};
                aabbs[*aabbCount].max = (Vector3){1.5f, (x + 1), (z + 1)};
                if (aabbs[*aabbCount - 1].min.x + 1 == aabbs[*aabbCount].min.x)
                { // Merge AABBs
                    aabbs[*aabbCount - 1].max = aabbs[*aabbCount].max;
                    aabbs = (AABB *)realloc(aabbs, sizeof(AABB) * (*aabbCount - 1));
                    continue;
                }
                (*aabbCount)++;
            }
        }
    }
    return aabbs;
}

void initGeometry(AABB **aabbs, int *aabbCount, int **maze, int size)
{
    *aabbs = generateAABBs(maze, size, aabbCount);
}