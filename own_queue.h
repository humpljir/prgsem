#ifndef __OWN_QUEUE_H__
#define __OWN_QUEUE_H__

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
    void *next;

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

extern queue event_queue;

void queue_cleanup();

event *queue_pop();

void queue_push(event *ev);

#endif