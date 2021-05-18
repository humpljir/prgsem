#include "keyboard.h"
#include "event_queue.h"
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
        if ((c = getchar()) == 's' || c == 'e' || c == 'b' || c=='q' || c == 'h' || (c >= 1 && c <= 5) || c == 'x')
        {
            switch (c)
            {
            case 'q':
                q.quit=true;
                break;
            
            default:
                break;
            }
            printf("%c\n",c);
        }
    }
    set_raw(false);
    pthread_exit(NULL);
}