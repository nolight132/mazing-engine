#ifndef VECTOR_H
#define VECTOR_H

#include <types.h>

Vector2 normalizeVector2(Vector2 v);
Vector3 normalizeVector3(Vector3 v);
#define normalize(v) _Generic((v), Vector2: normalizeVector2, Vector3: normalizeVector3)(v);
Vector3 calculateDirection(Rotation rotation);

#endif // VECTOR_H