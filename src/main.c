#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "map.h"

void free2DArray(int **array, int rows);

int main()
{
    srand(time(NULL));
    int size = 33;
    int **map = generateMaze(size);
    printMap(map, size);

    free2DArray(map, size);
    return 0;
}

void renderLoop()
{
}

void free2DArray(int **array, int rows)
{
    for (int i = 0; i < rows; i++)
    {
        free(array[i]);
    }
    // Free the array of pointers
    free(array);
}