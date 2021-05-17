#include <stdlib.h>
#include <pthread.h>
#include "main.h"
#include "keyboard.h"
#include "tx.h"
#include "run_prgsem.h"

#define NUM_THREADS 3

int main()
{
    pthread_t threads[NUM_THREADS];

    pthread_create(&threads[0], NULL, main_thread, NULL);
    pthread_create(&threads[1], NULL, keyboard_thread, NULL);
    pthread_create(&threads[2], NULL, tx_thread, NULL);

    return 1;
}