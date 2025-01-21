#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "types.h"

typedef struct
{
    int size;
    int chunkSize;
    int score;
    double playTime;
    Map mapData;
    GeometryData geometry;
    Vector2 startPos;
    Vector3 startWorld;
} LevelData;

int checkForWin(Vector2 playerPos, Vector2 goal);
void victoryScreen(int score, double playTime);

#endif // GAMELOGIC_H
