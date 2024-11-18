#ifndef MAP_H
#define MAP_H

#include <stdbool.h>
#include <types.h>


int **generateMaze(int d);
void generatePath(int **map, int d);
Vector2D *getValidDirs(Vector2D pos, bool **visitedTiles, int d, int *validCount);
Vector2D getRandDir(Vector2D dirs[], int count);
void generateBranches(int **map, Vector2D pos, bool **visitedTiles, int d, Vector2D *path, int pathIndex);
void printMap(int **map, int size);
void printTile(int type);

#endif // MAP_H