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
                AABB candidate;
                candidate.min = (Vector3){0, x, z};
                candidate.max = (Vector3){2, (x + 1), (z + 1)};
                if (*aabbCount > 0 && aabbs[*aabbCount - 1].min.z == candidate.min.z &&
                    maze[(int)candidate.min.z][(int)candidate.min.x - 1] == WALL)
                { // Merge AABBs horizontally
                    aabbs[*aabbCount - 1].max = candidate.max;
                    continue;
                }
                else
                {
                    aabbs = (AABB *)realloc(aabbs, sizeof(AABB) * (*aabbCount + 1));
                    aabbs[*aabbCount] = candidate;
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