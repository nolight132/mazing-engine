#include "graphics/vector.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <types.h>

Vector2 calculateStart(int **map, int d)
{
    // Randomly choose a start
    Vector2 start;
    do
    {
        start.y = rand() % (d - 1);
        start.x = rand() % (d - 1);
    } while (map[(int)start.y][(int)start.x] != PATH);
    return (Vector2){start.y, start.x};
}

Vector2 calculateGoal(int **map, int d, Vector2 start)
{
    // Randomly choose a goal
    Vector2 goal;
    do
    {
        goal.y = rand() % (d - 1);
        goal.x = rand() % (d - 1);
    } while (map[(int)goal.y][(int)goal.x] != PATH || distance(start, goal) < (float)d / 2);
    return (Vector2){goal.y, goal.x};
}

Vector2 *getValidDirs(Vector2 pos, bool **visitedTiles, int d, int *validCount)
{
    Vector2 dirs[] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    Vector2 *validDirs = (Vector2 *)malloc(4 * sizeof(Vector2));
    *validCount = 0;
    for (int i = 0; i < 4; i++)
    {
        Vector2 target = {pos.y + dirs[i].y * 2, pos.x + dirs[i].x * 2};
        if (target.y > 0 && target.y < d && target.x > 0 && target.x < d && !visitedTiles[(int)target.y][(int)target.x])
        {
            validDirs[*validCount] = dirs[i];
            (*validCount)++;
        }
    }
    return validDirs;
}

Vector2 getRandDir(Vector2 dirs[], int count)
{
    if (count == 0)
    {
        return (Vector2){0, 0};
    }
    return dirs[rand() % count];
}

void generateBranches(int **map, Vector2 pos, bool **visitedTiles, int d, Vector2 path[], int pathIndex)
{
    int validCount = 0;
    Vector2 *validDirs = getValidDirs(pos, visitedTiles, d, &validCount);
    if (validCount > 0)
    {
        Vector2 randDir = getRandDir(validDirs, validCount);
        Vector2 target = {(int)pos.y + randDir.y * 2, (int)pos.x + randDir.x * 2};
        // Fill in the target
        map[(int)target.y][(int)target.x] = PATH;
        // Fill in the tile between the current pos and the target
        map[(int)target.y - (int)randDir.y][(int)target.x - (int)randDir.x] = PATH;
        pos = target;
        visitedTiles[(int)pos.y][(int)pos.x] = true;
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
    free(validDirs);
}

void generatePath(int **map, int d)
{
    if (d < 5)
        return;

    Vector2 pos = {floor(d / 2.0f), floor(d / 2.0f)}; // Fixed starting position
    Vector2 path[(int)(pow(floor(d / 2.0f), 2))];     // Max path length

    int pathIndex = 1;
    bool **visitedTiles = (bool **)malloc(d * sizeof(bool *));

    for (int i = 0; i < d; i++)
    {
        visitedTiles[i] = (bool *)malloc(d * sizeof(bool));
    }
    // Initialize visitedTiles with walls
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            visitedTiles[i][j] = false;
        }
    }

    visitedTiles[(int)pos.y][(int)pos.x] = true;
    map[(int)pos.y][(int)pos.x] = PATH;
    path[0] = pos;

    generateBranches(map, pos, visitedTiles, d, path, pathIndex);
    // Free visitedTiles memory
    for (int i = 0; i < d; i++)
    {
        free(visitedTiles[i]);
    }
    free(visitedTiles);
}

Map generateMaze(int d)
{
    // Initialize the map
    int **map = (int **)malloc(d * sizeof(int *));
    for (int y = 0; y < d; y++)
    {
        map[y] = (int *)malloc(d * sizeof(int));
    }
    generatePath(map, d);
    Vector2 start = calculateStart(map, d);
    map[(int)start.y][(int)start.x] = START;
    // Apply the goal
    Vector2 goal = calculateGoal(map, d, start);
    map[(int)goal.y][(int)goal.x] = GOAL;
    return (Map){map, d, start, goal};
}
