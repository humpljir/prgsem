#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "event_queue.h"
#include "main.h"

void *main_thread(void* d)
{
    pthread_exit(NULL);
}