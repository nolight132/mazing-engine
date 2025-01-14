#ifndef DEBUG_H
#define DEBUG_H

#include <graphics/camera.h>
#include <graphics/screen.h>
#include <stdio.h>

void printDebugInfo(Screen screen, Camera camera, GeometryData geometry, int size, long long frameTime,
                    long long sleepTime);
void printMap(int **map, int size, Vector2 playerPos);
void printTile(int type);
int logWrite(char *fmt, ...);
int consoleLog(char *message);
int errorLog(char *message);
int initLog(FILE *file);
FILE *createLog(char *logFilePath, char *latestLogFilePath);

#endif // DEBUG_H
