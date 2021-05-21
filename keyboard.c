#include "keyboard.h"
#include "own_queue.h"
#include "messages.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void set_raw(_Bool set)
{
    if (set)
    {
        system("stty raw -echo"); // enable raw, disable echo
    }
    else
    {
        system("stty -raw echo"); // disable raw, enable echo
    }
}

void *keyboard_thread(void *d)
{
    char c;
    set_raw(true);
    while (!(event_queue.quit))
    {
        pthread_mutex_unlock(&(event_queue.mtx));
        c = getchar();
        switch (c)
        {
        case 'q':
            pthread_mutex_lock(&(event_queue.mtx));
            event_queue.quit = true;
            pthread_mutex_unlock(&(event_queue.mtx));
            break;

        default:
        {
            event *ev = (event *)malloc(sizeof(event));
            ev->type = EV_KEYBOARD;
            ev->param = c;
            ev->msg = NULL;
            queue_push(ev);
            break;
        }
        }
        pthread_mutex_lock(&(event_queue.mtx));
    }
    
    set_raw(false);
    pthread_exit(NULL);
}