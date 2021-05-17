CFLAGS+= -Wall -Werror -fPIE -std=gnu99 -g
LDFLAGS=  -pthread 

HW=prgsem
BINARIES=prgsem

#LDFLAGS += -L/usr/local/lib -ljpeglib
#CXXFLAGS += -I/usr/local/include
CFLAGS+=$(shell sdl2-config --cflags)
LDFLAGS+=$(shell sdl2-config --libs) -lSDL2_image



all: ${BINARIES}

OBJS=${patsubst %.c,%.o,${wildcard *.c}}

prgsem: ${OBJS}
    ${CC} ${OBJS} ${CXXFLAGS}  ${LDFLAGS} -o $@

${OBJS}: %.o: %.c
    ${CC} -c ${CFLAGS}  $< -o $@

clean:
    rm -f ${BINARIES} ${OBJS}