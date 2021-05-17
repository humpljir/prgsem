#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void my_assert(bool r, const char *fcname, int line, const char *fname);
void* my_alloc(size_t size);

#endif