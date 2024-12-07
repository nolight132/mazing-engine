#include <graphics/camera.h>
#include <math.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <types.h>

void updateGeometry(GeometryData *geometry, Camera camera)
{
    geometry->currentChunkZ = (int)camera.position.z / geometry->defaultChunkSize;
    geometry->currentChunkX = (int)camera.position.x / geometry->defaultChunkSize;
}

GeometryData generateAABBs(int **maze, int size, int chunkSize)
{
    int chunkCountRow = size / (float)chunkSize;
    int *chunkSizeData = (int *)malloc(sizeof(int) * chunkCountRow * chunkCountRow);
    AABB **aabbs = (AABB **)malloc(sizeof(AABB *) * chunkCountRow * chunkCountRow);
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
                    if (x > chunkX * chunkSize && maze[z][x - 1] == WALL &&
                        aabbs[i][chunkAabbCount - 1].min.z == candidate.min.z &&
                        aabbs[i][chunkAabbCount - 1].max.z == candidate.max.z &&
                        aabbs[i][chunkAabbCount - 1].max.x == x)
                    {
                        aabbs[i][chunkAabbCount - 1].max.x += 1;
                    }
                    // TODO : Try to merge vertically
                    else if (z != 0 && z > chunkZ * chunkSize && maze[z - 1][x] == WALL &&
                             ((x == size - 1) || (maze[z - 1][x + 1] != WALL)) && maze[z - 1][x - 1] != WALL)
                    {
                        for (int j = 0; j < chunkAabbCount; j++)
                        {
                            if (aabbs[i][j].min.x == x && aabbs[i][j].max.z == z)
                            {
                                aabbs[i][j].max.z = candidate.max.z;
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
    GeometryData data = {aabbs, pow(chunkCountRow, 2), chunkCountRow, chunkSize, chunkSizeData, 0, 0};
    return data;
}

void initGeometry(GeometryData *data, int chunkSize, int **maze, int size)
{
    *data = generateAABBs(maze, size, chunkSize);
}