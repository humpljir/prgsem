#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include "utils.h"

void my_assert(bool r, const char *fcname, int line, const char *fname)
{
if(!r)
{
    fprintf(stderr, "ERROR: my_assert FAIL %s() line %d in %s\n", fname, line, fname);
    exit(100);
}
}

void *my_alloc(size_t size)
{
    void *ret = malloc(size);
    if(!ret)
    {
        fprintf(stderr, "ERROR: cannot malloc!\n");
        exit(101);
    }
    return ret;
}

void call_termios(int reset)
{
    static struct termios tio, tioOld;
    tcgetattr(STDIN_FILENO,&tio);
    if(reset)
    {
        tcsetattr(STDIN_FILENO, TCSANOW, &tioOld);
    }    
    else
    {
        tioOld=tio;
        cfmakeraw(&tio);
        tcsetattr(STDIN_FILENO, TCSANOW, &tio);
    }
}