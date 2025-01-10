__kernel void computeRayKernel(
    __global const float4* cameraPos,  // Camera position (float4 for x, y, z, w=0)
    __global const float4* cameraDir,  // Camera direction (float4 for x, y, z, w=0)
    __global const int2* pixelCoords,  // Pixel coordinates (int2 for x, y)
    __global const int* screenDims,    // Screen dimensions (int2 for width, height)
    const float fov,                   // Field of view in radians
    __global float4* rayOrigins,       // Ray origins (output)
    __global float4* rayDirs)          // Ray directions (output)
{
    int id = get_global_id(0); // Each work-item processes one pixel
    int2 pixel = pixelCoords[id];
    int screenWidth = screenDims[0];
    int screenHeight = screenDims[1];

    // Compute aspect ratio and FOV scaling
    float aspectRatio = (float)screenWidth / (float)screenHeight;
    float tanHalfFovY = tan(fov * 0.5f);

    // Compute normalized screen space coordinates
    float pixelNdcY = (2.0f * (pixel.y + 0.5f) / screenHeight - 1.0f) * tanHalfFovY * aspectRatio;
    float pixelNdcX = (2.0f * (pixel.x + 0.5f) / screenWidth - 1.0f) * tanHalfFovY * aspectRatio;

    // Compute ray direction in world space
    float3 dir = vload3(0, &cameraDir[0]);
    float3 right = (float3)(0.0f, cos(dir.y), -sin(dir.y));
    right = normalize(right);
    float3 up = (float3)(-1.0f, 0.0f, 0.0f); // TODO: Update if using pitch

    float3 rayDir = normalize(dir + pixelNdcY * up - pixelNdcX * right);

    // Store the ray origin and direction
    vstore3(vload3(0, &cameraPos[0]), 0, &rayOrigins[id]);
    vstore3(rayDir, 0, &rayDirs[id]);
}

__kernel void raycastKernel(
    __global const float4* raysOrigin,  // Ray origins (float4 for x, y, z, w=0)
    __global const float4* raysDir,    // Ray directions (float4 for x, y, z, w=0)
    __global const float4* boxMin,     // Box minimum points (float4 for x, y, z, w=0)
    __global const float4* boxMax,     // Box maximum points (float4 for x, y, z, w=0)
    __global float* results,           // Results array (output)
    const int numRays)                 // Number of rays
{
    int id = get_global_id(0); // Each work-item processes one ray
    if (id >= numRays) return;

    // Load ray and box data
    float3 origin = vload3(0, &raysOrigin[id]);
    float3 dir = vload3(0, &raysDir[id]);
    float3 min = vload3(0, &boxMin[id]);
    float3 max = vload3(0, &boxMax[id]);

    // Ray-AABB intersection
    float t1 = (min.x - origin.x) / dir.x;
    float t2 = (max.x - origin.x) / dir.x;
    float t3 = (min.y - origin.y) / dir.y;
    float t4 = (max.y - origin.y) / dir.y;
    float t5 = (min.z - origin.z) / dir.z;
    float t6 = (max.z - origin.z) / dir.z;

    float tmin = fmax(fmax(fmin(t1, t2), fmin(t3, t4)), fmin(t5, t6));
    float tmax = fmin(fmin(fmax(t1, t2), fmax(t3, t4)), fmax(t5, t6));

    // Determine intersection result
    if (tmax < 0 || tmin > tmax) {
        results[id] = -1.0f; // No intersection
    } else {
        results[id] = (tmin < 0) ? tmax : tmin; // Return nearest intersection
    }
}
