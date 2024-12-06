#include "map/map.h"
#include "memory/memoryManagement.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <types.h>

int **copy2DArray(int **array, int size)
{
    int **copy = (int **)malloc(sizeof(int *) * size);
    for (int i = 0; i < size; i++)
    {
        copy[i] = (int *)malloc(sizeof(int) * size);
        for (int j = 0; j < size; j++)
        {
            copy[i][j] = array[i][j];
        }
    }
    return copy;
}

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
                    printf("Checking x: %d, z: %d\n", x, z);
                    if (x > chunkX * chunkSize && maze[z][x - 1] == WALL &&
                        aabbs[i][chunkAabbCount - 1].min.z == candidate.min.z &&
                        aabbs[i][chunkAabbCount - 1].max.z == candidate.max.z &&
                        aabbs[i][chunkAabbCount - 1].max.x == x)
                    {
                        printf("Success. Merging horizontally...\n");
                        printf("Previous AABB: min: %.2f,%.2f max: %.2f,%.2f\n", aabbs[i][chunkAabbCount - 1].min.x,
                               aabbs[i][chunkAabbCount - 1].min.z, aabbs[i][chunkAabbCount - 1].max.x,
                               aabbs[i][chunkAabbCount - 1].max.z);
                        aabbs[i][chunkAabbCount - 1].max.x += 1;
                    }
                    // TODO : Try to merge vertically
                    else if (z != 0 && z > chunkZ * chunkSize && maze[z - 1][x] == WALL &&
                             ((x == size - 1) || (maze[z - 1][x + 1] != WALL)) && maze[z - 1][x - 1] != WALL)
                    {
                        printf("chunksize %d, chunkAabbCount %d\n", chunkSize, chunkAabbCount);
                        int **copyMap = copy2DArray(maze, size);
                        copyMap[z][x] = BORDER;
                        printMap(copyMap, size);
                        free2DArray((void **)copyMap, size);
                        for (int j = 0; j < chunkAabbCount; j++)
                        {
                            if (aabbs[i][j].min.x == x && aabbs[i][j].max.z == z)
                            {
                                aabbs[i][j].max.z = candidate.max.z;
                                printf("WE EMERGE VICTORIOUS!!!! WE DID IT LADS!! Merging vertically...\nNew AABB: "
                                       "min: %.2f,%.2f max: %.2f,%.2f\n",
                                       aabbs[i][j].min.x, aabbs[i][j].min.z, aabbs[i][j].max.x, aabbs[i][j].max.z);
                                break;
                            }
                        }
                    }
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