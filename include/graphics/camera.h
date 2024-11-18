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

void cameraMove(Camera *camera, Vector3D direction);
void cameraRotate(Camera *camera, Rotation rotation);
void initCamera(Camera *camera, int fov, Vector3D position, Rotation rotation);


#endif // CAMERA_H