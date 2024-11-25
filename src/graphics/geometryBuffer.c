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
                AABB *temp = (AABB *)realloc(aabbs, sizeof(AABB) * (*aabbCount + 1));
                if (temp == NULL)
                {
                    printf("Memory allocation failed\n");
                    free(aabbs);
                    exit(1);
                }
                else
                {
                    aabbs = temp;
                }
                aabbs[*aabbCount].min = (Vector3){0, x, z};
                aabbs[*aabbCount].max = (Vector3){1, (x + 1), (z + 1)};
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