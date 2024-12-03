#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <types.h>

GeometryData generateAABBs(int **maze, int size, int chunkSize)
{
    int chunkCount = pow(size / (float)chunkSize, 2);
    int *chunkSizeData = (int *)malloc(sizeof(int) * chunkCount);
    AABB **aabbs = (AABB **)malloc(sizeof(AABB *) * chunkCount);
    int i = 0;
    for (int chunkZ = 0; chunkZ < size / chunkSize; chunkZ++)
    {
        for (int chunkX = 0; chunkX < size / chunkSize; chunkX++, i++)
        {
            aabbs[i] = NULL;
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
                    // printf("Checking x: %d, z: %d\n", x, z);
                    if (x > chunkX * chunkSize && maze[z][x - 1] == WALL)
                    {
                        // printf("Success. Merging horizontally...\n");
                        aabbs[i][chunkAabbCount - 1].max.x += 1;
                    }
                    // TODO : Try to merge vertically
                    // else if (z > chunkZ * chunkSize && maze[z - 1][x] == WALL && maze[z - 1][x + 1] != WALL &&
                    //          maze[z - 1][x - 1] != WALL)
                    // {
                    //     // printf("Success. Merging vertically...\n");
                    //     for (int j = chunkSize; j < chunkAabbCount; j++)
                    //     {
                    //         if (aabbs[i][j].min.x == candidate.min.x && aabbs[i][j].min.z == candidate.min.z - 1)
                    //         {
                    //             aabbs[i][j].max.z += 1;
                    //             break;
                    //         }
                    //     }
                    // }
                    // Create new AABB
                    else
                    {
                        // printf("Creating aabb...%d\n", chunkAabbCount);
                        aabbs[i] = (AABB *)realloc(aabbs[i], sizeof(AABB) * (chunkAabbCount + 1));
                        aabbs[i][chunkAabbCount] = candidate;
                        chunkAabbCount++;
                    }
                }
            }
            chunkSizeData[i] = chunkAabbCount;
        }
    }
    GeometryData data = {aabbs, chunkCount, chunkSizeData};
    return data;
}

void initGeometry(GeometryData *data, int chunkSize, int **maze, int size)
{
    *data = generateAABBs(maze, size, chunkSize);
}