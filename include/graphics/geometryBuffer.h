#ifndef GEOMETRYBUFFER_H
#define GEOMETRYBUFFER_H

#include <types.h>

void initGeometry(int **maze, int size);
AABB *generateAABBs(int **maze, int size, int *aabbCount);

#endif // GEOMETRYBUFFER_H