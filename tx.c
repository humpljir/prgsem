#include "tx.h"
#include "event_queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void *tx_thread(void* d)
{
    pthread_exit(NULL);
}