#include "matrix.h"
#include <stdlib.h>

matrix matrix_init(int re, int im)
{
    matrix results;
    int *arr=(int *) malloc(re*im * sizeof(int));
    results.table = arr;

    return results;
}
