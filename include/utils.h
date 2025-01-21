#ifndef UTILS_H
#define UTILS_H

#include <graphics/camera.h>
#include <graphics/screen.h>
#include <ncurses.h>
#include <stdio.h>

void drawDebugInfo(WINDOW *win, Screen screen, Camera camera, GeometryData geometry, int size, long long frameTime,
                   long long sleepTime, double totalPlayTime);
void printMap(int **map, int size, Vector2 playerPos);
void printTile(int type);
int logWrite(char *fmt, ...);
int consoleLog(char *message);
int errorLog(char *message);
int initLog(FILE *file);
FILE *createLog(char *logFilePath, char *latestLogFilePath);

#endif // UTILS_H
