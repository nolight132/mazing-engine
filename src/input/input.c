#include <graphics/camera.h>
#include <graphics/vector.h>
#include <math.h>
#include <ncurses.h>
#include <types.h>

Vector3 acceleration = {0, 0, 0};
float speed = 0.05f;
float maxSpeed = 0.05f;
float drag = 50.0f;

void applyMovementDrag(Vector3 *acceleration, float drag, float deltaTime)
{
    float vLength = vectorLength(*acceleration);
    if (vLength > 0.0f)
    {
        Vector3 dragVector = multiplyVectorByFloat(*acceleration, -drag * deltaTime);
        *acceleration = addVector3(*acceleration, dragVector);
    }
}

void handleInput(int input, Camera *camera, double deltaTime)
{
    Vector3 cameraDirection = calculateDirection(camera->rotation);
    Vector3 up = (Vector3){1, 0, 0};
    Vector3 left = crossProduct(up, cameraDirection);
    left = normalize(left);
    Vector3 right = crossProduct(cameraDirection, up);
    right = normalize(right);

    if (input != ERR)
    {
        float vLength = vectorLength(acceleration);
        switch (input)
        {
            case 'w': // Move forward
                if (vLength < maxSpeed)
                    acceleration = addVector(acceleration, multiplyVectorByFloat(cameraDirection, speed));
                break;
            case 'a': // Strafe left
                if (vLength < maxSpeed)
                    acceleration = addVector(acceleration, multiplyVectorByFloat(right, speed));
                break;
            case 's': // Move backward
                if (vLength < maxSpeed)
                    acceleration = subtractVector(acceleration, multiplyVectorByFloat(cameraDirection, speed));
                break;
            case 'd': // Strafe right
                if (vLength < maxSpeed)
                    acceleration = subtractVector(acceleration, multiplyVectorByFloat(right, speed));
                break;
            case KEY_LEFT:
                camera->rotation.yaw += M_PI / 4;
                break;
            case KEY_RIGHT:
                camera->rotation.yaw -= M_PI / 4;
                break;
        }
    }
    camera->position = addVector3(camera->position, acceleration);
    applyMovementDrag(&acceleration, drag, deltaTime);
}