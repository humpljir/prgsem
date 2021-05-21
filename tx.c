#include <stdio.h>
#include "messages.h"
#include "tx.h"

void *tx_thread(void *d)
{
    return NULL;
}

/*

void *serial_rx_thread(void *d) 
{ // reads bytes from the serial and puts the parsed message to the queue
  shared_data_t *shared = (shared_data_t *) d;
  uint8_t msg_buf[sizeof(message)]; // maximal buffer for all possible messages defined in messages.h
  event ev = {.source = EV_NUCLEO, .type = EV_SERIAL, .data.msg = NULL};
  bool end = false;
  bool send_quit = false; // indicates whether the reason for quit comes from this thread
  unsigned char c;
  pthread_mutex_lock(&mtx);
  int fd = shared->fd;
  pthread_mutex_unlock(&mtx);
  while (serial_getc_timeout(fd, SERIAL_READ_TIMEOUT_MS, &c) > 0); // discard garbage

  while (!end)
    {
      int r = serial_getc_timeout(fd, SERIAL_READ_TIMEOUT_MS, &c);  //TODO: make timeout shorter
      if (r > 0)
        {
          int msg_size;
          if (get_message_size(c, &msg_size)) // type recognized
            {
              msg_buf[0] = c;
              for (int i = 1; i < msg_size; i++)
                {
                  r = serial_getc_timeout(fd,
                                          SERIAL_READ_TIMEOUT_MS,
                                          &c);
                  if (r > 0)  // successfully read
                    {
                      msg_buf[i] = c;
                    }
                  else
                    {
                      fprintf(stderr, "WARNING: error reading message from nucleo 1\n");
                      break;
                    }
                }
              if (r > 0) // successfully read the whole length
                {
                  // allocate space for msg 
                  message *msg = (message *) malloc(sizeof(message));
                  if (parse_message_buf(msg_buf, msg_size, msg)) // cksum ok
                    {
                      ev.source = EV_NUCLEO;
                      ev.data.msg = msg;
                      queue_push(ev);  // push event to queue
                      if (DEBUG_MSG) fprintf(stderr, "DEBUG: successfully received incoming message from nucleo 1\n");
                    }
                  else
                    {
                      free(msg);
                      fprintf(stderr, "WARNING: wrong cksum of incoming message from 1\n");
                    }
                  ev.data.msg = NULL;
                }
            } // type not recognized by get_message_size
          else
            {
              fprintf(stderr, "WARNING: couldn't recognize incoming message type from nucleo 1\n");
            }
        }
      else if (r == 0)  //didn't read nothing
        {
          // TODO: nothing
        }
      else
        {
          fprintf(stderr, "ERROR: Can't receive data from the serial port: nucleo 1\n");
          send_quit = true;
        }
      pthread_mutex_lock(&mtx);
      end = send_quit || shared->quit;
      pthread_mutex_unlock(&mtx);
    } // end of while
  if (send_quit)  // quiting caused by this thread
    {
      ev.type = EV_QUIT;
      queue_push(ev);
    }
  fprintf(stderr, "INFO: Exit serial_rx_thread %p\n", (void *) pthread_self());
  return NULL;
}

*/