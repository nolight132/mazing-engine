#include <math.h>
#include <stdlib.h>
#include <types.h>

AABB **generateAABBs(int **maze, int size, int *aabbCount, int chunkSize)
{
    int chunkCount = pow(size / (float)chunkSize, 2);
    if (size / chunkSize < 2)
    {
        chunkSize = size;
        chunkCount = 1;
    }
    AABB **aabbs = (AABB **)malloc(sizeof(AABB *) * chunkCount);
    for (int i = 0; i < chunkCount; i++)
    {
        aabbs[i] = NULL;
    }
    int z = 0;
    int x = 0;
    for (int i = 0; i < chunkCount; i++)
    {
        int chunkAabbCount = 0;
        for (; z < chunkSize; z++)
        {
            for (; x < chunkSize; x++)
            {
                if (maze[z][x] == WALL)
                {
                    AABB candidate;
                    candidate.min = (Vector3){0, x, z};
                    candidate.max = (Vector3){2, (x + 1), (z + 1)};
                    if (chunkAabbCount > 0 && aabbs[i][chunkAabbCount - 1].min.z == candidate.min.z &&
                        maze[(int)candidate.min.z][(int)candidate.min.x - 1] == WALL)
                    { // Merge AABBs horizontally
                        aabbs[i][chunkAabbCount - 1].max = candidate.max;
                    }
                    else if (z > 0 && maze[z - 1][x] == WALL && maze[z - 1][x + 1] != WALL &&
                             maze[z - 1][x - 1] != WALL)
                    { // Merge AABBs vertically
                        for (int j = 1; j < chunkAabbCount; j++)
                        {
                            if (aabbs[i][j].max.z == candidate.max.z - 1 && aabbs[j][i].max.x == candidate.max.x)
                            {
                                aabbs[i][j].max = candidate.max;
                                break;
                            }
                        }
                    }
                    else
                    {
                        aabbs[i] = (AABB *)realloc(aabbs[i], sizeof(AABB) * (chunkAabbCount + 1));
                        aabbs[i][chunkAabbCount] = candidate;
                        chunkAabbCount++;
                        (*aabbCount)++;
                    }
                }
            }
        }
    }
    return aabbs;
}

void initGeometry(AABB ***aabbs, int *aabbCount, int **maze, int size)
{
    *aabbs = generateAABBs(maze, size, aabbCount, 8);
}