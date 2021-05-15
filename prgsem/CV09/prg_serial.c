/*
 * File name: prg_serial.c
 * Date:      2017/04/07 22:34
 * Author:    Jan Faigl
 */

#include <assert.h>

#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

#include "prg_serial.h"

#ifndef BAUD_RATE
#define BAUD_RATE B115200
#endif

/// ----------------------------------------------------------------------------
int serial_open(const char *fname)
{
   int openOK = 1;
   int fd = open(fname, O_RDWR | O_NOCTTY | O_SYNC);
   openOK = fd != -1;
   struct termios term;
   if (openOK) {
      openOK = tcgetattr(fd, &term) >= 0;
   }
   if (openOK) {
      cfmakeraw(&term);
      term.c_cc[VTIME] = 2; //set vtime 
      term.c_cc[VMIN] = 0;
      cfsetispeed(&term, BAUD_RATE);
      cfsetospeed(&term, BAUD_RATE);
      openOK = tcsetattr(fd, TCSADRAIN, &term) >= 0;
   }
   openOK = openOK && fcntl(fd, F_GETFL) >= 0;
   openOK = openOK && tcsetattr(fd, TCSADRAIN, &term) >= 0;
   openOK = openOK && fcntl(fd, F_GETFL) >= 0;
   if (openOK) {
      tcflush(fd, TCIFLUSH);
      tcsetattr(fd, TCSANOW, &term);
   }

   /* Set the serial port to non block mode
   int flags = fcntl(fd, F_GETFL);
   flags &= ~O_NONBLOCK;
   assert(fcntl(fd, F_SETFL, flags) >= 0);
   */
   return openOK ? fd : -1;
}

/// ----------------------------------------------------------------------------
int serial_close(int fd)
{
   return close(fd);
}

/// ----------------------------------------------------------------------------
int serial_putc(int fd, char c)
{
   return write(fd, &c, 1);
}

/// ----------------------------------------------------------------------------
int serial_getc(int fd)
{
   char c;
   int r = read(fd, &c, 1);
   return r == 1 ? c : -1;
}

/* end of prg_serial.c */
