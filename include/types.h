#ifndef TYPES_H
#define TYPES_H

enum TileType
{
    WALL,
    PATH,
    BORDER,
    GOAL,
    START
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
    float yaw;   // Rotation around the y-axis
    float pitch; // Rotation around the x-axis
} Rotation;

typedef struct
{
    AABB **aabbs;
    int chunkCount;
    int chunkCountRow;
    int defaultChunkSize;
    int *chunkSizeData;
    int currentChunkX;
    int currentChunkZ;
} GeometryData;

typedef struct
{
    int **map;
    int size;
    Vector2 start;
    Vector2 goal;
} Map;

#endif // TYPES_H
