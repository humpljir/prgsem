#include "matrix.h"

matrix matrix_init(int re, int im)
{
    matrix results;
    int *arr[re];
     results.table = arr;
     
    for (int i = 0; i < re; i++)
    {
        results.table[i] = (int *)malloc(im * sizeof(int));
    }

    return results;
}
