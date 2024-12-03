#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <types.h>

AABB **generateAABBs(int **maze, int size, int chunkSize, int *chunkCount)
{
    *chunkCount = pow(size / (float)chunkSize, 2);
    AABB **aabbs = (AABB **)malloc(sizeof(AABB *) * *chunkCount);
    int i = 0;
    for (int chunkZ = 0; chunkZ < size / chunkSize; chunkZ++)
    {
        for (int chunkX = 0; chunkX < size / chunkSize; chunkX++, i++)
        {
            int chunkAabbCount = 0;
            for (int z = chunkZ * chunkSize; z < (chunkZ + 1) * chunkSize; z++)
            {
                for (int x = chunkX * chunkSize; x < (chunkX + 1) * chunkSize; x++)
                {
                    if (maze[z][x] != WALL)
                    {
                        continue;
                    }
                    AABB candidate;
                    candidate.min = (Vector3){0, x, z};
                    candidate.max = (Vector3){2, (x + 1), (z + 1)};

                    aabbs[i] = (AABB *)realloc(aabbs[i], sizeof(AABB) * (chunkAabbCount + 1));
                    aabbs[i][chunkAabbCount] = candidate;
                    chunkAabbCount++;
                }
            }
        }
    }
    return aabbs;
}

void initGeometry(AABB ***aabbs, int chunkSize, int *chunkCount, int **maze, int size)
{
    *aabbs = generateAABBs(maze, size, chunkSize, chunkCount);
}