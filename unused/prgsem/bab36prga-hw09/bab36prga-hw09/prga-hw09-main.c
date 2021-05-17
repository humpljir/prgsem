/*
 * File name: prga-hw09-main.c
 * Date:      2017/04/14 18:51
 * Author:    Jan Faigl
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <termios.h>
#include <unistd.h> // for STDIN_FILENO
#include <pthread.h>

#include "xwin_sdl.h"

#include "prg_io_nonblock.h"
#include "messages.h"
#include "event_queue.h"

// terminal settings
void call_termios(int reset);
// assigns color to calculated fractals
void redraw(int w, int h, uint8_t *grid, uint8_t threshold, unsigned char *out);

// TODO: add supporting threads functions

// - main ---------------------------------------------------------------------
int main(int argc, char *argv[])
{
	// TODO: open pipe

	call_termios(0);

	// TODO: create and initialize threads, mutex and conditional variables

	struct
	{
		uint16_t w; // resolution
		uint16_t h; // resolution
		bool computing;
		// TODO: add necessary items
	} computation = {
		.w = 640, .h = 480
	};
	// TODO: create and initialize computation structure

    // TODO: initialize GUI
	xwin_init(computation.w, computation.h);

    // Main thread
	bool quit = false;
	while (!quit)
	{
		event ev = queue_pop();
		xwin_poll_events(); //restore possible shadowed window in ubuntu by reading all pending window events
		if (ev.source == EV_KEYBOARD)
		{
			// TODO: handle events from keyboard
		}
		else if (ev.source == EV_NUCLEO)
		{ 
			// TODO: handle nucleo events
		}
	} // end main 


	xwin_close();
	call_termios(1); // restore terminal settings

	return EXIT_SUCCESS;
}

// - function -----------------------------------------------------------------
void call_termios(int reset)
{
	static struct termios tio, tioOld;
	tcgetattr(STDIN_FILENO, &tio);
	if (reset)
	{
		tcsetattr(STDIN_FILENO, TCSANOW, &tioOld);
	}
	else
	{
		tioOld = tio; //backup
		cfmakeraw(&tio);
		tio.c_oflag |= OPOST;
		tcsetattr(STDIN_FILENO, TCSANOW, &tio);
	}
}

// - function -----------------------------------------------------------------
void redraw(int w, int h, uint8_t *grid, uint8_t threshold, unsigned char *out)
{
	int nsize = w * h;
	unsigned char *cur = out;
	for (int i = 0; i < nsize; ++i)
	{
		const int n = *(grid++);
		const double t = 1. * n / threshold;
		if (t < threshold)
		{
			*(cur++) = (int)(9 * (1 - t) * t * t * t * 255);
			*(cur++) = (int)(15 * (1 - t) * (1 - t) * t * t * 255);
			*(cur++) = (int)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);
		}
		else
		{
			for (int j = 0; j < 3; ++j)
			{
				*(cur++) = 0;
			}
		}
	}
}

/* end of prga-hw09-main.c */
