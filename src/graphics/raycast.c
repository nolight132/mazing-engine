#include <OpenCL/opencl.h>
#include <float.h>
#include <graphics/camera.h>
#include <graphics/screen.h>
#include <graphics/vector.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <types.h>

// Function to load OpenCL kernel source from a file
const char *loadKernelSource(const char *fileName)
{
    FILE *file = fopen(fileName, "r");
    if (!file)
    {
        fprintf(stderr, "Failed to load kernel file: %s\n", fileName);
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);

    char *source = (char *)malloc(fileSize + 1);
    if (!source)
    {
        fprintf(stderr, "Failed to allocate memory for kernel source.\n");
        exit(EXIT_FAILURE);
    }

    fread(source, 1, fileSize, file);
    source[fileSize] = '\0'; // Null-terminate the string
    fclose(file);

    return source;
}

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
    // Vector3 intersectionPoint = addVector(ray.origin, multiplyVectorByFloat(ray.direction, tmin));
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

    // Calculate camera basis vectors
    Vector3 right = (Vector3){0, cos(camera.rotation.yaw), -sin(camera.rotation.yaw)};
    right = normalize(right);
    Vector3 up = (Vector3){-1, 0, 0}; // Assumes 'up' is constant, probably a TODO if I end up doing pitch

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

float renderCall(GeometryData geometry, Camera camera, Screen screen, int pixelRow, int pixelCol)
{
    // OpenCL setup
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel computeRayKernel, raycastKernel;
    cl_int err;

    // Initialize OpenCL
    clGetPlatformIDs(1, &platform, NULL);
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
    context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
    queue = clCreateCommandQueueWithPropertiesAPPLE(context, device, 0, &err);

    // Load and build kernels
    const char *kernelSource =
        loadKernelSource("/Users/pavelolizko/Documents/Code/mazing-engine/src/graphics/kernels.cl");
    program = clCreateProgramWithSource(context, 1, &kernelSource, NULL, &err);
    clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    computeRayKernel = clCreateKernel(program, "computeRayKernel", &err);
    raycastKernel = clCreateKernel(program, "raycastKernel", &err);

    // Buffers
    size_t pixelCount = screen.width * screen.height;
    cl_mem cameraBuffer =
        clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(Camera), &camera, &err);
    cl_mem geometryBuffer =
        clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(GeometryData), &geometry, &err);
    cl_mem resultBuffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * pixelCount, NULL, &err);

    // Set arguments for raycasting kernel
    clSetKernelArg(raycastKernel, 0, sizeof(cl_mem), &geometryBuffer);
    clSetKernelArg(raycastKernel, 1, sizeof(cl_mem), &cameraBuffer);
    clSetKernelArg(raycastKernel, 2, sizeof(cl_mem), &resultBuffer);
    clSetKernelArg(raycastKernel, 3, sizeof(int), &pixelRow);
    clSetKernelArg(raycastKernel, 4, sizeof(int), &pixelCol);

    // Execute raycasting kernel
    size_t globalWorkSize = geometry.chunkCountRow * geometry.chunkCountRow;
    clEnqueueNDRangeKernel(queue, raycastKernel, 1, NULL, &globalWorkSize, NULL, 0, NULL, NULL);

    // Retrieve results
    float *results = (float *)malloc(sizeof(float) * pixelCount);
    clEnqueueReadBuffer(queue, resultBuffer, CL_TRUE, 0, sizeof(float) * pixelCount, results, 0, NULL, NULL);

    // Find the minimum t value for this pixel
    float tmin = FLT_MAX;
    for (size_t i = 0; i < pixelCount; i++)
    {
        if (results[i] < tmin && results[i] >= 0)
        {
            tmin = results[i];
        }
    }

    // Cleanup
    clReleaseMemObject(cameraBuffer);
    clReleaseMemObject(geometryBuffer);
    clReleaseMemObject(resultBuffer);
    clReleaseKernel(computeRayKernel);
    clReleaseKernel(raycastKernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
    free(results);

    return (tmin == FLT_MAX) ? -1.0f : tmin;
}
