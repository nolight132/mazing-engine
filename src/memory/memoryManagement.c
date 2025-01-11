#include <stdlib.h>
#include <types.h>

void free2DArray(void **array, int rows)
{
    for (int i = 0; i < rows; i++)
    {
        free(array[i]);
    }
    // Free the array of pointers
    free(array);
}