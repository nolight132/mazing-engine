#include <graphics/camera.h>
#include <graphics/raycast.h>
#include <graphics/vector.h>
#include <math.h>
#include <ncurses.h>
#include <stdio.h>
#include <types.h>

Vector3 velocity = {0, 0, 0};
float speed = 0.05f;
float maxSpeed = 0.05f;
float drag = 50.0f;

void applyMovementDrag(Vector3 *acceleration, float drag, float deltaTime)
{
    float vLength = vectorLength(*acceleration);
    if (vLength > 0.0f)
    {
        float decayFactor = expf(-drag * deltaTime); // Exponential decay
        *acceleration = multiplyVectorByFloat(*acceleration, decayFactor);
    }
}

bool canGoThrough(Camera camera, Vector3 velocity, GeometryData geometry)
{
    // int currentChunkIndex = geometry.currentChunkZ * geometry.chunkCountRow + geometry.currentChunkX;
    Vector3 normalizedVelocity = normalize(velocity);
    Ray r = {camera.position, normalizedVelocity};
    float distance = 0.5f;
    // TODO: OPTIMIZE
    for (int dx = -camera.renderDistance; dx <= camera.renderDistance; dx++)
    {
        for (int dz = -camera.renderDistance; dz <= camera.renderDistance; dz++)
        {
            int chunkX = geometry.currentChunkX + dx;
            int chunkZ = geometry.currentChunkZ + dz;

            // Check bounds to ensure valid chunk indices
            if (chunkX < 0 || chunkX >= geometry.chunkCountRow || chunkZ < 0 || chunkZ >= geometry.chunkCountRow)
            {
                continue;
            }

            // Calculate the corresponding chunk index in the linear array
            int chunkIndex = chunkZ * geometry.chunkCountRow + chunkX;
            for (int i = 0; i < geometry.chunkSizeData[chunkIndex]; i++)
            {
                float newDistance = raycast(r, geometry.aabbs[chunkIndex][i]);
                if (newDistance == -1)
                {
                    continue;
                }
                else if (newDistance < distance)
                {
                    distance = newDistance;
                }
            }
        }
    }
    if (distance > 0.1f)
    {
        return true;
    }
    return false;
}

void handleInput(int input, Camera *camera, GeometryData geometry, double deltaTime)
{
    Vector3 cameraDirection = calculateDirection(camera->rotation);
    Vector3 up = (Vector3){1, 0, 0};
    Vector3 left = crossProduct(up, cameraDirection);
    left = normalize(left);
    Vector3 right = crossProduct(cameraDirection, up);
    right = normalize(right);

    if (input != ERR)
    {
        float vLength = vectorLength(velocity);
        switch (input)
        {
            case 'w': // Move forward
                if (vLength < maxSpeed)
                    velocity = addVector(velocity, multiplyVectorByFloat(cameraDirection, speed));
                break;
            case 'a': // Strafe left
                if (vLength < maxSpeed)
                    velocity = addVector(velocity, multiplyVectorByFloat(right, speed));
                break;
            case 's': // Move backward
                if (vLength < maxSpeed)
                    velocity = subtractVector(velocity, multiplyVectorByFloat(cameraDirection, speed));
                break;
            case 'd': // Strafe right
                if (vLength < maxSpeed)
                    velocity = subtractVector(velocity, multiplyVectorByFloat(right, speed));
                break;
            case KEY_LEFT:
                camera->rotation.yaw += M_PI / 4;
                break;
            case KEY_RIGHT:
                camera->rotation.yaw -= M_PI / 4;
                break;
        }
    }
    if (!canGoThrough(*camera, velocity, geometry))
    {
        velocity = (Vector3){0, 0, 0};
    }
    else
    {
        camera->position = addVector3(camera->position, velocity);
        applyMovementDrag(&velocity, drag, deltaTime);
    }
}