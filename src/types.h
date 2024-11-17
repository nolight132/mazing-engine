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
} Vector2D;

typedef struct
{
    float y;
    float x;
    float z;
} Vector3D;

typedef struct {
    Vector3D min; // Minimum corner of the box
    Vector3D max; // Maximum corner of the box
} AABB;

typedef struct
{
    double yaw;
    double pitch;
} Rotation;

#endif  // TYPES_H
