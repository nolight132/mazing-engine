#include <stdlib.h>

void free2DArray(int **array, int rows)
{
    for (int i = 0; i < rows; i++)
    {
        free(array[i]);
    }
    // Free the array of pointers
    free(array);
}