#ifndef MAP_H
#define MAP_H

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "types.h"

enum TileType
{
    WALL,
    PATH,
    BORDER
};

void generatePath(int **map, int d);
Vector2D *getValidDirs(Vector2D pos, bool **visitedTiles, int d, int *validCount);
Vector2D getRandDir(Vector2D dirs[], int count);
void generateBranches(int **map, Vector2D pos, bool **visitedTiles, int d, Vector2D *path, int pathIndex);
void printMap(int **map, int size);
void printTile(int type);

int **generateMaze(int d)
{
    // Initialize the map
    int **map = (int **)malloc(d * sizeof(int *));
    for (int y = 0; y < d; y++)
    {
        map[y] = (int *)malloc(d * sizeof(int));
    }
    // Add walls around the map
    for (int i = 0; i < d; i++)
    {
        map[0][i] = map[i][0] = map[d - 1][i] = map[i][d - 1] = BORDER;
    }
    generatePath(map, d);
    return map;
}
void generatePath(int **map, int d)
{
    if (d < 5)
        return;

    Vector2D pos = {(rand() % ((d - 3) / 2)) * 2 + 2, (rand() % ((d - 3) / 2)) * 2 + 2};
    Vector2D path[(int)(pow(floor(d / 2), 2))];

    int pathIndex = 1;
    bool **visitedTiles = (bool **)malloc(d * sizeof(bool *));

    for (int i = 0; i < d; i++)
    {
        visitedTiles[i] = (bool *)malloc(d * sizeof(bool));
    }
    // Initialize visitedTiles with false values
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            visitedTiles[i][j] = false;
        }
    }

    visitedTiles[pos.y][pos.x] = true;
    map[pos.y][pos.x] = PATH;
    path[0] = pos;

    generateBranches(map, pos, visitedTiles, d, path, pathIndex);
    // Free visitedTiles memory
    for (int i = 0; i < d; i++)
    {
        free(visitedTiles[i]);
    }
    free(visitedTiles);
}
void generateBranches(int **map, Vector2D pos, bool **visitedTiles, int d, Vector2D path[], int pathIndex)
{
    int validCount = 0;
    Vector2D *validDirs = getValidDirs(pos, visitedTiles, d, &validCount);
    if (validCount > 0)
    {
        Vector2D randDir = getRandDir(validDirs, validCount);
        Vector2D target = {pos.y + randDir.y * 2, pos.x + randDir.x * 2};
        // Fill in the target
        map[target.y][target.x] = PATH;
        // Fill in the tile between the current pos and the target
        map[target.y - randDir.y][target.x - randDir.x] = PATH;
        pos = target;
        visitedTiles[pos.y][pos.x] = true;
        // Add pos to path
        path[pathIndex] = pos;
        (pathIndex)++;
        generateBranches(map, pos, visitedTiles, d, path, pathIndex);
    }
    else if (pathIndex > 0)
    {
        pathIndex--;
        generateBranches(map, path[pathIndex], visitedTiles, d, path, pathIndex);
    }
    return;
}
Vector2D *getValidDirs(Vector2D pos, bool **visitedTiles, int d, int *validCount)
{
    Vector2D dirs[] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    Vector2D *validDirs = (Vector2D *)malloc(4 * sizeof(Vector2D));
    *validCount = 0;
    for (int i = 0; i < 4; i++)
    {
        Vector2D target = {pos.y + dirs[i].y * 2, pos.x + dirs[i].x * 2};
        if (target.y > 0 && target.y < d - 1 && target.x > 0 && target.x < d - 1 && !visitedTiles[target.y][target.x])
        {
            validDirs[*validCount] = dirs[i];
            (*validCount)++;
        }
    }
    return validDirs;
}
Vector2D getRandDir(Vector2D dirs[], int count)
{
    if (count == 0)
    {
        return (Vector2D){0, 0};
    }
    return dirs[rand() % count];
}
void printMap(int **map, int size)
{
    for (int y = 0; y < size; y++)
    {
        for (int x = 0; x < size; x++)
        {
            printTile(map[y][x]);
        }
        printf("\n");
    }
}
void printTile(int type)
{
    switch (type)
    {
        case PATH:
            printf("  ");
            break;
        case WALL:
            printf("░░");
            break;
        case BORDER:
            printf("██");
            break;
    }
}

#endif // MAP_H
