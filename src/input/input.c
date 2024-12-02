#include <graphics/camera.h>
#include <graphics/vector.h>
#include <math.h>
#include <ncurses.h>
#include <types.h>

Vector3 acceleration = {0, 0, 0};
float speed = 0.005f;
float maxAcceleration = 0.01f;
float drag = 0.05f;

void applyDrag(Vector3 *acceleration, float drag, float deltaTime)
{
    if (acceleration->y != 0)
        acceleration->y -= (acceleration->y > 0 ? drag : -drag) * deltaTime;

    if (acceleration->x != 0)
        acceleration->x -= (acceleration->x > 0 ? drag : -drag) * deltaTime;

    if (acceleration->z != 0)
        acceleration->z -= (acceleration->z > 0 ? drag : -drag) * deltaTime;
}

void handleInput(char input, Camera *camera, double deltaTime)
{
    Vector3 cameraDirection = calculateDirection(camera->rotation);
    Vector3 up = (Vector3){1, 0, 0};
    Vector3 left = crossProduct(up, cameraDirection);
    left = normalize(left);
    Vector3 right = crossProduct(cameraDirection, up);
    right = normalize(right);

    if (input != ERR)
    {
        switch (input)
        {
            case 'w': // Move forward
                if (fabs(acceleration.y) < maxAcceleration)
                    acceleration = addVector(acceleration, multiplyVectorByFloat(cameraDirection, speed));
                break;
            case 'a': // Strafe left
                if (fabs(acceleration.x) < maxAcceleration)
                    acceleration = subtractVector(acceleration, multiplyVectorByFloat(right, speed));
                break;
            case 's': // Move backward
                if (fabs(acceleration.y) < maxAcceleration)
                    acceleration = subtractVector(acceleration, multiplyVectorByFloat(cameraDirection, speed));
                break;
            case 'd': // Strafe right
                if (fabs(acceleration.x) < maxAcceleration)
                    acceleration = addVector(acceleration, multiplyVectorByFloat(right, speed));
                break;
        }
    }
    camera->position = addVector3(camera->position, acceleration);
    applyDrag(&acceleration, drag, deltaTime);
}