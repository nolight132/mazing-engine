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

void cameraMove(Camera *camera, Vector3 position);
void cameraRotate(Camera *camera, Vector3 direction);
void initCamera(Camera *camera, int fov, Vector3 position, Vector3 direction);

#endif // CAMERA_H