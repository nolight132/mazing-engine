#include <stdlib.h>
#include <types.h>

AABB *generateAABBs(int **maze, int size, int *aabbCount)
{
    AABB *aabbs = (AABB *)malloc(sizeof(AABB));
    aabbs[0] = (AABB){(Vector3){-2.5f, -2.0f, -2.0f}, (Vector3){-2.5f, size + 2.0f, size + 2.0f}}; // Floor
    *aabbCount = 1;
    for (int z = 0; z < size; z++)
    {
        for (int x = 0; x < size; x++)
        {
            if (maze[z][x] == WALL)
            {
                AABB candidate;
                candidate.min = (Vector3){0, x, z};
                candidate.max = (Vector3){2, (x + 1), (z + 1)};
                if (aabbs[*aabbCount - 1].min.z == candidate.min.z &&
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
                            aabbs[i].max = candidate.max;
                            break;
                        }
                    }
                }
                else
                {
                    aabbs = (AABB *)realloc(aabbs, sizeof(AABB) * (*aabbCount + 1));
                    aabbs[*aabbCount] = candidate;
                    (*aabbCount)++;
                }
            }
        }
    }
    return aabbs;
}

void initGeometry(AABB **aabbs, int *aabbCount, int **maze, int size)
{
    *aabbs = generateAABBs(maze, size, aabbCount);
}