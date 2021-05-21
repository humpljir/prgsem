#include "own_queue.h"

queue event_queue = {.size = 0, .start = NULL, .end = NULL, .quit = false};

void queue_cleanup(void)
{

}

event *queue_pop()
{
    if (event_queue.size > 0)
    {
        event *ret = event_queue.start;
        event_queue.start = event_queue.start->next;
        event_queue.size--;
        
        return ret;
    }
    else
    {
        return NULL;
    }
}

void queue_push(event* ev)
{
    pthread_mutex_lock(&(event_queue.mtx));
    //event *ev = (event *)malloc(sizeof(event));
    ev->next = NULL;

    if (event_queue.size <= 0)
    {
        event_queue.start=ev; 
    }
    else
    {
        event_queue.end->next=ev;
    }

    event_queue.end = ev;
    event_queue.size++;
    pthread_mutex_unlock(&(event_queue.mtx));
}