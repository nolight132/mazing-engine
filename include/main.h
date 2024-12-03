#ifndef MAIN_H
#define MAIN_H

#include <graphics/camera.h>
#include <graphics/screen.h>
#include <stdbool.h>
#include <time.h>
#include <types.h>

void deltaUpdate(Screen *screen, Camera *camera, AABB **aabbs, int input, Vector2 mouseDelta, double deltaTime);
bool gameRunning();
// Function to calculate the time difference (delta time) in seconds between two times
double calculateDeltaTime(struct timespec start, struct timespec end);
void debugPrintAABB(AABB **aabbs, int chunkCount, int *chunkSizeData);

#endif // MAIN_H