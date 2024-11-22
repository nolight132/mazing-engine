#ifndef TYPES_H
#define TYPES_H

enum TileType
{
    WALL,
    PATH,
    BORDER
};

typedef struct
{
    float y;
    float x;
} Vector2;

typedef struct
{
    float y;
    float x;
    float z;
} Vector3;

typedef struct {
    Vector3 min; // Minimum corner of the box
    Vector3 max; // Maximum corner of the box
} AABB;

typedef struct
{
    double yaw; // Rotation around the y-axis
    double pitch; // Rotation around the x-axis
} Rotation;

#endif  // TYPES_H
