#ifndef __MATRIX_H__
#define __MATRIX_H__

typedef struct
{
    int re;
    int im;
    int *table;
} matrix;

matrix matrix_init(int re, int im);

#endif