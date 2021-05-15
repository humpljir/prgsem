#include <stdio.h>
#include <stdbool.h>
#include <termios.h>
#include <unistd.h>
#include <pthread.h>
#include "prg_serial.h"

#define MS 1000
#define PERIOD_THREAD 300
#define myDEVICE "/dev/ttyS4"

void* outputThread(void*);
void* computeThread(void*);
void* inputThread(void*);
void call_termios(int reset);


typedef struct {
    int period;
    int counter;
    bool quit;
    bool is_serial_open;
} SharedData;

SharedData initData() {
    SharedData ret;
    ret.period = PERIOD_THREAD;
    ret.counter = 0;
    ret.quit = false;
    ret.is_serial_open = false;
    return ret;
}

pthread_mutex_t mtx;
pthread_cond_t condvar;

int main(void)
{
    SharedData data = initData();
    call_termios(0);

    pthread_mutex_init(&mtx, NULL);
    pthread_cond_init(&condvar, NULL);

    enum { COMPUTE, OUTPUT, INPUT, NUM_THREADS };
    void* (*thr_threads[])(void*) = { computeThread, outputThread, inputThread };
    pthread_t threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, thr_threads[i], &data);

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }
    call_termios(1);
    printf("\rMain thread finished, counter: - %4i\n", data.counter);
    return 0;
}

void* computeThread(void *v)
{
    SharedData* data = (SharedData*) v;
    bool q = false;
    while (!q) {
        pthread_mutex_lock(&mtx);
        int per = data->period;
        pthread_mutex_unlock(&mtx);
        usleep(per * MS);
        pthread_mutex_lock(&mtx);
        data->counter += 1;
        pthread_cond_signal(&condvar);
        q = data->quit;
        pthread_mutex_unlock(&mtx);
    }
    printf("\rCompute thread finished\n");
    return 0;
}

void* outputThread(void *v) 
{
    SharedData* data = (SharedData*) v;
    
    const char* device = myDEVICE;
    fprintf(stderr, "INFO: open serial port at %s\n", device);
    int fd = serial_open(device);
    bool q = false;
    if (fd == -1) {
        fprintf(stderr, "\rERROR: Cannot open device %s\n", device);
        pthread_mutex_lock(&mtx);
        data->quit = true;
        pthread_mutex_unlock(&mtx);
    }
    pthread_mutex_lock(&mtx);
    data->is_serial_open = true;
    pthread_mutex_unlock(&mtx);
    while (!q) {
        pthread_mutex_lock(&mtx);
        pthread_cond_wait(&condvar, &mtx);
        int tmp = data->counter;
        int per = data->period;
        q = data->quit;
        pthread_mutex_unlock(&mtx);
        if (!q) {
            printf("\rCounter %10i, period: %10i", tmp, per);
            fflush(stdout);
        }
    }

    if (fd != -1)
        serial_close(fd);
    printf("\rOutput thread finished\n");
    return 0;
} 

void* inputThread(void *v) 
{
    SharedData* data = (SharedData*) v;
    bool qq = false;
    bool q = false;
    while (!qq && !q) {
        pthread_mutex_lock(&mtx);
        qq = data->is_serial_open;
        q = data->quit;
        pthread_mutex_unlock(&mtx);
    }
    while (!q) {
        char inC = getchar();
        if (inC == 'q') {
            pthread_mutex_lock(&mtx);
            data->quit = true;
            q = data->quit;
            pthread_mutex_unlock(&mtx);
        }
        pthread_mutex_lock(&mtx);
        int per = data->period;
        switch (inC) {
            case 'p':
                data->period += 5;
                if (data->period > 2000)
                    data->period = 2000;
                break;
            case 'm':
                data->period -= 5;
                if (data->period < 20)
                    data->period = 20;
                break;
        }
        if (per != data->period)
            pthread_cond_broadcast(&condvar);
        pthread_mutex_unlock(&mtx);
    }
    printf("\rInput thread finished\n");
    return 0;
} 

void call_termios(int reset)
{
    static struct termios tio, tioOld; // use static to preserve the initial settings
    tcgetattr(STDIN_FILENO, &tio);
    if (reset) {
        tcsetattr(STDIN_FILENO, TCSANOW, &tioOld);
    } else {
        tioOld = tio; //backup
        cfmakeraw(&tio);
        tcsetattr(STDIN_FILENO, TCSANOW, &tio);
    }
}
