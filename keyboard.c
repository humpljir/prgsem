#include "keyboard.h"
#include "event_queue.h"
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
    while (!(q.quit))
    {
        pthread_mutex_unlock(&(q.mtx));
        if ((c = getchar()) == 'g' || c == 'e' || c == 'b' || c == 'q' || c == 'h' || (c >= 1 && c <= 5) || c == 'x')
        {
            switch (c)
            {
            case 'g':
                /*
            ***NEFUNGUJE*** ale je to pekny
                event ev;
                ev.source = EV_KEYBOARD;
                ev.type = EV_GET_VERSION;
                ev.data.msg->type = MSG_GET_VERSION;
                queue_push(ev);
            */
                event ev = {.source = EV_KEYBOARD, .type = EV_GET_VERSION};
                queue_push(ev);

                break;

            case 'q':
                pthread_mutex_lock(&(q.mtx));
                q.quit = true;
                pthread_mutex_unlock(&(q.mtx));
                break;

            default:
                break;
            }
            printf("%c\n", c);
        }
        pthread_mutex_lock(&(q.mtx));
    }
    set_raw(false);
    pthread_exit(NULL);
}