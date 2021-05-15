#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define LENGTH 100

int main(int argc, char const *argv[])
{
    int pipe;
    const char *pipename = "/tmp/pipe";
    mkfifo(pipename, 0666); // create pipe with read/write permissions for everyone

    char buf_write[LENGTH];
    char buf_read[LENGTH];

    // Read from pipe
    // Uncomment this part to see how you can write to pipe... 
    // Open terminal and read from pipe
    // pipe = open(pipename, O_WRONLY); // O_WRONGLY - write only flag    
    // printf("Write a line:\n");
    // fgets(buf_write, LENGTH, stdin);
    // write(pipe, buf_write, strlen(buf_write));
    // close(pipe);

    // Write to pipe
    // Uncomment this part to see how you can read from pipe... 
    // Open terminal and write to pipe
    // pipe = open(pipename, O_RDONLY); // read inly flag
    // read(pipe, buf_read, LENGTH);
    // printf("Line from pipe: '%s'\n", buf_read);
    // close(pipe);

    return 0;
}

