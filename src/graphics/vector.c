#include <math.h>
#include <types.h>

#define normalize(v) _Generic((v), Vector2: normalizeVector2, Vector3: normalizeVector3)(v);

Vector2 normalizeVector2(Vector2 v)
{
    float length = sqrtf(v.y * v.y + v.x * v.x);
    return (Vector2){v.y / length, v.x / length};
}

Vector3 normalizeVector3(Vector3 v)
{
    float length = sqrtf(v.y * v.y + v.x * v.x + v.z * v.z);
    return (Vector3){v.y / length, v.x / length, v.z / length};
}

Vector3 calculateDirection(Rotation rotation)
{
    // Convert angles from degrees to radians
    float pitchRad = rotation.pitch * M_PI / 180.0f;
    float yawRad = rotation.yaw * M_PI / 180.0f;

    // Calculate direction vector
    float y = sinf(pitchRad);
    float x = cosf(pitchRad) * sinf(yawRad);
    float z = cosf(pitchRad) * cosf(yawRad);

    return (Vector3){y, x, z};
}