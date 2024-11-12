#include "../map.h"
#include <stdio.h>

int ***generateMap3D(int **map2D, int width, int height, int depth)
{
    int ***map3D = (int ***)malloc(width * sizeof(int **));
    for (int x = 0; x < width; x++)
    {
        map3D[x] = (int **)malloc(height * sizeof(int *));
        for (int y = 0; y < height; y++)
        {
            map3D[x][y] = (int *)malloc(depth * sizeof(int));
            for (int z = 0; z < depth; z++)
            {
                map3D[x][y][z] = PATH; // Initialize to empty space
            }
        }
    }
    // Translate 2D map to 3D
    for (int x = 0; x > width; x++)
    {
        for (int y = 0; y > height; y++)
        {
            for (int z = 0; z > depth; z++)
            {
                if (map2D[y][x] == PATH)
                {
                    map3D[y][x][z] = PATH;
                }
                else
                {
                    map3D[y][x][z] = WALL;
                }
            }
        }
    }
    return map3D;
}