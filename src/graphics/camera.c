#ifndef CAMERA_H
#define CAMERA_H

#include <types.h>

typedef struct Camera
{
    int fov;
    Vector3D position;
    Rotation rotation;

    // Function pointers
    void (*move)(struct Camera *, Vector3D);
    void (*rotate)(struct Camera *, Rotation);
    void (*init)(struct Camera *, int, Vector3D, Rotation);
} Camera;

void cameraMove(Camera *camera, Vector3D direction)
{
    camera->position.x += direction.x;
    camera->position.y += direction.y;
    camera->position.z += direction.z;
}

void cameraRotate(Camera *camera, Rotation rotation)
{
    camera->rotation = rotation;
}

void initCamera(Camera *camera, int fov, Vector3D position, Rotation rotation)
{
    camera->fov = fov;
    camera->position = position;
    camera->rotation = rotation;
    camera->move = cameraMove;
    camera->rotate = cameraRotate;
    camera->init = initCamera;
}

#endif // CAMERA_H
