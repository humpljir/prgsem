#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "main.h"
#include "keyboard.h"
#include "tx.h"
#include "run.h"
#include "own_queue.h"

#define NUM_THREADS 3

int main()
{
    pthread_t threads[NUM_THREADS];

    if(pthread_create(&threads[0], NULL, main_thread, NULL) + pthread_create(&threads[1], NULL, keyboard_thread, NULL) + pthread_create(&threads[2], NULL, tx_thread, NULL) != 0)
    {
        fprintf(stderr, "ERROR: Unable to create threads, terminating the program!\n");
        return 0;
    }

    for (int i = 0; i < NUM_THREADS; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&(event_queue.mtx));
    queue_cleanup();
    
    return EXIT_SUCCESS;
}