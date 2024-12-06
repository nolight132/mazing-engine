#include <graphics/camera.h>
#include <graphics/screen.h>
#include <graphics/vector.h>
#include <math.h>
#include <stdbool.h>
#include <types.h>

float raycast(Ray ray, AABB box)
{
    float t1 = (box.min.x - ray.origin.x) / ray.direction.x;
    float t2 = (box.max.x - ray.origin.x) / ray.direction.x;
    float t3 = (box.min.y - ray.origin.y) / ray.direction.y;
    float t4 = (box.max.y - ray.origin.y) / ray.direction.y;
    float t5 = (box.min.z - ray.origin.z) / ray.direction.z;
    float t6 = (box.max.z - ray.origin.z) / ray.direction.z;

    float tmin = fmaxf(fmaxf(fminf(t1, t2), fminf(t3, t4)), fminf(t5, t6));
    float tmax = fminf(fminf(fmaxf(t1, t2), fmaxf(t3, t4)), fmaxf(t5, t6));

    // if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behing us
    if (tmax < 0)
    {
        return -1;
    }

    // if tmin > tmax, ray doesn't intersect AABB
    if (tmin > tmax)
    {
        return -1;
    }

    if (tmin < 0)
    {
        return tmax;
    }
    Vector3 intersectionPoint = addVector(ray.origin, multiplyVectorByFloat(ray.direction, tmin));
    return tmin;
}

// Function to compute a ray from the camera
Ray computeRay(Camera camera, Screen screen, int pixelRow, int pixelCol)
{
    // Compute the aspect ratio and FOV scaling factors
    Vector3 direction = calculateDirection(camera.rotation);
    float fov = (float)camera.fov * (M_PI / 180.0f);
    float aspectRatio = (float)screen.width / (float)screen.height;
    float tanHalfFovY = tanf(fov * 0.5f);

    // Calculate the camera basis vectors
    Vector3 right = (Vector3){0, cos(camera.rotation.yaw), -sin(camera.rotation.yaw)};
    right = normalize(right);

    Vector3 up = (Vector3){-1, 0, 0};
    up = normalize(up);

    // Map pixel coordinates to normalized screen space (-1 to 1)
    float pixelNdcY = (2.0f * (pixelRow + 0.5f) / screen.height - 1.0f) * tanHalfFovY * aspectRatio;
    float pixelNdcX = (2.0f * (pixelCol + 0.5f) / screen.width - 1.0f) * tanHalfFovY * aspectRatio;

    // Compute the direction in world space
    Vector3 pixelWorldDir =
        addVector(direction, addVector(multiplyVectorByFloat(up, pixelNdcY), multiplyVectorByFloat(right, -pixelNdcX)));
    pixelWorldDir = normalize(pixelWorldDir);

    // Create the ray
    Ray ray = {camera.position, pixelWorldDir};
    return ray;
}

float raycastCall(GeometryData data, Camera camera, Screen screen, int pixelRow, int pixelCol)
{
    int rowChunkCount = (int)sqrt(data.chunkCount);

    int currentChunkX = (int)(camera.position.x / data.defaultChunkSize);
    int currentChunkY = (int)(camera.position.z / data.defaultChunkSize);

    float tmin = INFINITY;
    Ray ray = computeRay(camera, screen, pixelRow, pixelCol);
    float t;

    // Iterate over adjacent chunks (current + neighbors)
    for (int dx = -1; dx <= 1; dx++)
    {
        for (int dy = -1; dy <= 1; dy++)
        {
            int chunkX = currentChunkX + dx;
            int chunkY = currentChunkY + dy;

            // Check bounds to ensure valid chunk indices
            if (chunkX < 0 || chunkX >= rowChunkCount || chunkY < 0 || chunkY >= rowChunkCount)
            {
                continue;
            }

            // Calculate the corresponding chunk index in the linear array
            int chunkIndex = chunkY * rowChunkCount + chunkX;

            // Iterate through the objects in this chunk
            for (int j = 0; j < data.chunkSizeData[chunkIndex]; j++)
            {
                t = raycast(ray, data.aabbs[chunkIndex][j]);
                if (t <= tmin && t >= 0)
                {
                    tmin = t;
                }
            }
        }
    }

    return tmin == INFINITY ? -1 : tmin;
}
