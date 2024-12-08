#ifndef VECTOR_H
#define VECTOR_H

#include <types.h>

float vector2Length(Vector2 v);
float vector3Length(Vector3 v);
Vector2 normalizeVector2(Vector2 v);
Vector3 normalizeVector3(Vector3 v);
Vector2 addVector2(Vector2 a, Vector2 b);
Vector3 addVector3(Vector3 a, Vector3 b);
Vector2 subtractVector2(Vector2 a, Vector2 b);
Vector3 subtractVector3(Vector3 a, Vector3 b);
Vector2 multiplyVector2(Vector2 a, Vector2 b);
Vector3 multiplyVector3(Vector3 a, Vector3 b);
Vector2 multiplyVector2ByFloat(Vector2 v, float scalar);
Vector3 multiplyVector3ByFloat(Vector3 v, float scalar);
Vector3 crossProduct(Vector3 a, Vector3 b);
float dotVector2(Vector2 a, Vector2 b);
float dotVector3(Vector3 a, Vector3 b);
Vector3 calculateDirection(Rotation rotation);
#define vectorLength(v) _Generic((v), Vector2: vector2Length, Vector3: vector3Length)(v);
#define normalize(v) _Generic((v), Vector2: normalizeVector2, Vector3: normalizeVector3)(v);
#define addVector(a, b) _Generic((a), Vector2: addVector2, Vector3: addVector3)((a), (b))
#define subtractVector(a, b) _Generic((a), Vector2: subtractVector2, Vector3: subtractVector3)((a), (b))
#define multiplyVector(a, b) _Generic((a), Vector2: multiplyVector2, Vector3: multiplyVector3)((a), (b))
#define multiplyVectorByFloat(v, scalar)                                                                               \
    _Generic((v), Vector2: multiplyVector2ByFloat, Vector3: multiplyVector3ByFloat)((v), (scalar))
#define dot(a, b) _Generic((v), Vector2: dotVector2, Vector3: dotVector3)((a), (b))

#endif // VECTOR_H