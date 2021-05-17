#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // sleep()

#define N 3 // number of threads

pthread_mutex_t mtx; // mutex
pthread_cond_t condvar; // conditional variable

int counter = 0;

/**
 * Prints the value of the global variable 'counter' 
 * and the value of 'arg'.
 * @param arg: thread parameter represents ID of the thread
 * @return: NULL
 */
void *print_thread(void *arg);

/**
 * Increases the value of the global variable 'counter' while 
 * the value is less than LIMIT, and then sleeps for 1 ms.
 * @param arg: thread parameter represents ID of the thread
 * @return: NULL
 */ 
void *compute_thread(void *arg);

/**
 * Reads from the standartd input. Agter reading char 's' sets
 * the global variable 'quit' to 1.
 * @param arg: thread parameter represents ID of the thread
 * @return: NULL
 */ 
void *read_thread(void *arg);

int main(int argc, char const *argv[])
{
    pthread_t threads[N];
    for (int i = 0; i < N; i++) {
        pthread_create(&threads[i], NULL, print_thread, &i);
        counter = (int)i/2;
    }
    
    // TODO
    
    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], 0);
    }

    return 0;
}
