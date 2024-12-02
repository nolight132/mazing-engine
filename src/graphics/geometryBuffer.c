#include <math.h>
#include <stdlib.h>
#include <types.h>

AABB **generateAABBs(int **maze, int size, int *aabbCount, int chunkSize)
{
    AABB *aabbs = (AABB *)malloc(sizeof(AABB));
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
                }
                else if (z > 0 && maze[z - 1][x] == WALL && maze[z - 1][x + 1] != WALL && maze[z - 1][x - 1] != WALL)
                { // Merge AABBs vertically
                    for (int i = 1; i < *aabbCount; i++)
                    {
                        if (aabbs[i].max.z == candidate.max.z - 1 && aabbs[i].max.x == candidate.max.x)
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