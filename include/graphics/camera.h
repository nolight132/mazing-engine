#ifndef CAMERA_H
#define CAMERA_H

#include <types.h>

typedef struct Camera
{
    int fov;
    Vector3 position;
    Rotation rotation;

    // Function pointers
    void (*move)(struct Camera *, Vector3);
    void (*rotate)(struct Camera *, Rotation);
    void (*init)(struct Camera *, int, Vector3, Rotation);
} Camera;

void cameraMove(Camera *camera, Vector3 direction);
void cameraRotate(Camera *camera, Rotation rotation);
void initCamera(Camera *camera, int fov, Vector3 position, Rotation rotation);


#endif // CAMERA_H