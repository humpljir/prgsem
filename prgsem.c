#include <stdlib.h>
#include "prg_io_nonblock.h"
#include "utils.h"
#include "main.h"
#include "keyboard.h"
#include "prgsem.h"
#include "event_queue.h"

void *read_pipe_thread(void * d)
{
    return NULL;
}

int main(int argc, char *argv[])
{
    int ret = EXIT_SUCCESS;
    const char *fname_pipe_in = argc > 1 ? argv[1] : "/tmp/computational_module.out";
    const char *fname_pipe_out = argc > 2 ? argv[2] : "/tmp/computational_module.in";

    int pipe_in=io_open_read(fname_pipe_in);
    int pipe_out=io_open_write(fname_pipe_out);

    my_assert(pipe_in != -1 && pipe_out != -1,__func__,__LINE__,__FILE__);

    enum {KEYBOARD_THRD,READ_PIPE_THRD,MAIN_THRD,NUM_THREADS};

    void*(*thrd_functions[])(void*) = {keyboard_thread,read_pipe_thread,main_thread};

    io_close(pipe_in);
    io_close(pipe_out);

    return 0;
}