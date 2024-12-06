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

typedef struct
{
    Vector3 min; // Minimum corner of the box
    Vector3 max; // Maximum corner of the box
} AABB;

typedef struct
{
    Vector3 origin;
    Vector3 direction;
} Ray;

typedef struct
{
    double yaw;   // Rotation around the y-axis
    double pitch; // Rotation around the x-axis
} Rotation;

typedef struct
{
    AABB **aabbs;
    int chunkCount;
    int defaultChunkSize;
    int *chunkSizeData;
} GeometryData;

#endif // TYPES_H
