#include "../types.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct Camera
{
    int FOV;
    Vector3D position;
    Rotation rotation;

    // Function pointers
    void (*move)(struct Camera *, Vector3D);
    void (*rotate)(struct Camera *, Rotation);
    void (*init)(struct Camera *, int, Vector3D, Rotation);
} Camera;

void move(Camera *camera, Vector3D direction)
{
    camera->position.x += direction.x;
    camera->position.y += direction.y;
    camera->position.z += direction.z;
}

void rotate(Camera *camera, Rotation rotation)
{
    camera->rotation.yaw += rotation.yaw;
    camera->rotation.pitch += rotation.pitch;
}

void init(Camera *camera, int FOV, Vector3D position, Rotation rotation)
{
    camera->FOV = FOV;
    camera->position = position;
    camera->rotation = rotation;
    camera->move = move;
    camera->rotate = rotate;
    camera->init = init;
}
