#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <types.h>

GeometryData generateAABBs(int **maze, int size, int chunkSize, int *chunkCount)
{
    *chunkCount = pow(size / (float)chunkSize, 2);
    int *chunkSizeData = (int *)malloc(sizeof(int) * *chunkCount);
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

                    // Try to merge horizontally
                    if (x > chunkSize * i && maze[z][x - 1] == WALL)
                    {
                        printf("Merging horizontally x: %d, z: %d\n", x, z);
                        aabbs[i][chunkAabbCount - 1].max.x += 1;
                    }
                    // Try to merge vertically
                    else if (z > chunkZ * chunkSize && maze[z - 1][x] == WALL)
                    {
                        for (int j = 0; j < chunkAabbCount; j++)
                        {
                            if (aabbs[i][j].min.x == candidate.min.x && aabbs[i][j].max.x == candidate.max.x &&
                                aabbs[i][j].max.z == candidate.min.z)
                            {
                                printf("Merging vertically\n");
                                aabbs[i][j].max.z += 1;
                                break;
                            }
                        }
                    }
                    // Create new AABB
                    else
                    {
                        printf("create aabb %d\n", chunkAabbCount);
                        aabbs[i] = (AABB *)realloc(aabbs[i], sizeof(AABB) * (chunkAabbCount + 1));
                        aabbs[i][chunkAabbCount] = candidate;
                        chunkAabbCount++;
                    }
                }
            }
            chunkSizeData[i] = chunkAabbCount;
        }
    }
    GeometryData data = {aabbs, chunkSizeData};
    return data;
}

void initGeometry(AABB ***aabbs, int chunkSize, int **chunkSizeData, int *chunkCount, int **maze, int size)
{
    GeometryData data = generateAABBs(maze, size, chunkSize, chunkCount);
    *aabbs = data.aabbs;
    *chunkSizeData = data.chunkSizeData;
}