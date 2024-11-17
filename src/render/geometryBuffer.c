#ifndef GEOMETRYBUFFER_H
#define GEOMETRYBUFFER_H

#include "../types.h"
#include <stdio.h>
#include <stdlib.h>

// Example vertex array and index array
Vertex *vertices = NULL;
Triangle *triangles = NULL;
int vertexCount = 0;
int triangleCount = 0;

// Adds a vertex and returns its index
int addVertex(float x, float y, float z)
{
    vertices = (Vertex *)realloc(vertices, sizeof(Vertex) * (vertexCount + 1));
    vertices[vertexCount] = (Vertex){x, y, z};
    return vertexCount++;
}

// Adds a triangle using vertex indices
void addTriangle(int a, int b, int c)
{
    triangles = (Triangle *)realloc(triangles, sizeof(Triangle) * (triangleCount + 1));
    triangles[triangleCount++] = (Triangle){a, b, c};
}

void generateMazeGeometry(int **maze, int size, float wallHeight)
{
    for (int z = 0; z < size; z++)
    {
        for (int x = 0; x < size; x++)
        {
            if (maze[z][x] == 0)
            { // Wall cell
                // Add vertices for a cube
                int v0 = addVertex(x, 0, z);                  // Bottom-left front
                int v1 = addVertex(x + 1, 0, z);              // Bottom-right front
                int v2 = addVertex(x, wallHeight, z);         // Top-left front
                int v3 = addVertex(x + 1, wallHeight, z);     // Top-right front
                int v4 = addVertex(x, 0, z + 1);              // Bottom-left back
                int v5 = addVertex(x + 1, 0, z + 1);          // Bottom-right back
                int v6 = addVertex(x, wallHeight, z + 1);     // Top-left back
                int v7 = addVertex(x + 1, wallHeight, z + 1); // Top-right back

                // Add triangles for cube faces
                // Front face
                addTriangle(v0, v2, v1);
                addTriangle(v1, v2, v3);
                // Back face
                addTriangle(v4, v5, v6);
                addTriangle(v5, v7, v6);
                // Left face
                addTriangle(v0, v4, v2);
                addTriangle(v2, v4, v6);
                // Right face
                addTriangle(v1, v3, v5);
                addTriangle(v3, v7, v5);
                // Top face
                addTriangle(v2, v6, v3);
                addTriangle(v3, v6, v7);
                // Bottom face
                addTriangle(v0, v1, v4);
                addTriangle(v4, v1, v5);
            }
        }
    }
}

#endif // GEOMETRYBUFFER_H