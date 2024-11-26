#ifndef CAMERA_H
#define CAMERA_H

#include <types.h>

typedef struct Camera
{
    int fov;
    Vector3 position;
    Vector3 direction;

    // Function pointers
    void (*move)(struct Camera *, Vector3);
    void (*rotate)(struct Camera *, Vector3);
    void (*init)(struct Camera *, int, Vector3, Vector3);
} Camera;

void cameraMove(Camera *camera, Vector3 position)
{
    camera->position.x += position.x;
    camera->position.y += position.y;
    camera->position.z += position.z;
}

void cameraRotate(Camera *camera, Vector3 direction)
{
    camera->direction = direction;
}

void initCamera(Camera *camera, int fov, Vector3 position, Vector3 direction)
{
    camera->fov = fov;
    camera->position = position;
    camera->direction = direction;
    camera->move = cameraMove;
    camera->rotate = cameraRotate;
    camera->init = initCamera;
}

#endif // CAMERA_H
