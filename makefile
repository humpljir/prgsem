CC=gcc
CFLAGS=-lpthread -I -lSDL2_image.
DEPS = run_prgsem.h event_queue.h keyboard.h main.h messages.h tx.h
OBJ = run_prgsem.c event_queue.c keyboard.c main.c messages.c tx.c

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

make: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)