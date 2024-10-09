#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "map.h"

void free2DArray(int **array, int rows);

int main()
{
    clock_t start = clock();

    srand(time(NULL));
    int size = 33;
    int **map = generateMaze(size);
    // printMap(map, size);

    free2DArray(map, size);
    clock_t end = clock();

    // double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    // printf("Took %fs\n", time_spent);

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
