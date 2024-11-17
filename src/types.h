#ifndef TYPES_H
#define TYPES_H

typedef struct
{
    float y;
    float x;
} Vector2D;

typedef struct {
    float x, y, z;
} Vertex;

typedef struct {
    int a, b, c;  // Indices for the vertices of a triangle
} Triangle;

typedef struct
{
    float y;
    float x;
    float z;
} Vector3D;

typedef struct
{
    double yaw;
    double pitch;
} Rotation;

#endif  // TYPES_H
