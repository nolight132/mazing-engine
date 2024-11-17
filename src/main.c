#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include "map/map.h"
#include "memoryManagement.h"
#include "render/3dbuffer.h"

void free2DArray(int **array, int rows);

int main()
{
    srand(time(NULL));
    int size = 17;
    int **maze = generateMaze(size);
    generateMazeGeometry(maze, size, 1.0f);

    // Print results (for debugging)
    printf("Vertices (%d):\n", vertexCount);
    for (int i = 0; i < vertexCount; i++)
    {
        printf("  (%f, %f, %f)\n", vertices[i].x, vertices[i].y, vertices[i].z);
    }
    printMap(maze, size);

    // printf("Triangles (%d):\n", triangleCount);
    // for (int i = 0; i < triangleCount; i++)
    // {
    //     printf("  (%d, %d, %d)\n", triangles[i].a, triangles[i].b, triangles[i].c);
    // }

    // Initialize ncurses
    initscr();
    cbreak();              // Disable line buffering
    noecho();              // Disable echoing of typed characters
    keypad(stdscr, TRUE);  // Enable special keys
    curs_set(0);           // Hide the default cursor
    nodelay(stdscr, TRUE); // Make getch non-blocking

    const int frameDuration = 1000000 / 60; // 16.67 ms per frame

    endwin();

    // Free memory
    free(vertices);
    free(triangles);
    free2DArray(maze, size);
    return 0;
}