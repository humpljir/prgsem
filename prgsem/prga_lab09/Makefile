CFLAGS += -std=c99 -Wall  -pedantic -g
LFLAGS += -lpthread

all: main 
main: main.c
	$(CC) $(CFLAGS) main.c -o main $(LFLAGS)

clean:
	rm main