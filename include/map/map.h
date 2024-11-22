#ifndef MAP_H
#define MAP_H

#include <stdbool.h>
#include <types.h>


int **generateMaze(int d);
void generatePath(int **map, int d);
Vector2 *getValidDirs(Vector2 pos, bool **visitedTiles, int d, int *validCount);
Vector2 getRandDir(Vector2 dirs[], int count);
void generateBranches(int **map, Vector2 pos, bool **visitedTiles, int d, Vector2 *path, int pathIndex);
void printMap(int **map, int size);
void printTile(int type);

#endif // MAP_H