#ifndef GEOMETRYBUFFER_H
#define GEOMETRYBUFFER_H

#include <graphics/camera.h>
#include <types.h>

void initGeometry(GeometryData *data, int chunkSize, int **maze, int size);
void updateGeometry(GeometryData *geometry, Camera camera);

#endif // GEOMETRYBUFFER_H