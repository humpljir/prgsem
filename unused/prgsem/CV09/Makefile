CFLAGS+= -Wall -Werror -g
LDFLAGS=-pthread

BINARIES=main
all: $(BINARIES)

OBJS=$(patsubst %.c,%.o,$(wildcard *.c))

man: main

main: ${OBJS}
	${CC} ${LDFLAGS} main.c prg_serial.o -o $@

${OBJS}: %.o: %.c
	${CC} -c ${CFLAGS} $< -o $@
	
clean:
	rm -f ${BINARIES} ${OBJS}
