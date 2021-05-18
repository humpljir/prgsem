#ifndef __EVENT_QUEUE_H__
#define __EVENT_QUEUE_H__

#ifndef QUEUE_CAPACITY
#define QUEUE_CAPACITY 32
#endif

#include <pthread.h>
#include "messages.h"

typedef enum
{
    EV_KEYBOARD,
    EV_MESSAGE,
    EV_TYPE_NUM
} event_type;

typedef struct
{
    event_type type;
    uint8_t param;
    message *msg;
} event;

typedef struct
{
    int size;
    event *start;
    event *end;

    pthread_mutex_t mtx;
    pthread_cond_t cond;
    bool quit;
} queue;

queue q = {.size = 0, .start = NULL, .end = NULL, .quit = false};

void queue_cleanup(void);

event queue_pop(void);

void queue_push(event ev);

#endif