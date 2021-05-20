#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "own_queue.h"
#include "messages.h"
#include "main.h"

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define MESSAGE_SIZE (sizeof(message))
#define BUFFER_SIZE 100

/*

void Tx_interrupt();
void Rx_interrupt();
bool send_buffer(const uint8_t *msg, unsigned int size);
bool receive_message(uint8_t *msg_buf, int size, int *len);
bool send_message(const message *msg, uint8_t *buf, int size);

#define BUF_SIZE 255

char tx_buffer[BUF_SIZE];
char rx_buffer[BUF_SIZE];

// pointers to the circular buffers
volatile int tx_in = 0;
volatile int tx_out = 0;
volatile int rx_in = 0;
volatile int rx_out = 0;

#define MESSAGE_SIZE (sizeof(message))
#define FLIP_PERIOD 0.3

*/

int set_interface_attribs(int fd, int speed, int parity)
{
    struct termios tty;
    if (tcgetattr(fd, &tty) != 0)
    {
        fprintf(stderr,"error %d from tcgetattr", errno);
        return -1;
    }

    cfsetospeed(&tty, speed);
    cfsetispeed(&tty, speed);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8-bit chars
    // disable IGNBRK for mismatched speed tests; otherwise receive break
    // as \000 chars
    tty.c_iflag &= ~IGNBRK; // disable break processing
    tty.c_lflag = 0;        // no signaling chars, no echo,
                            // no canonical processing
    tty.c_oflag = 0;        // no remapping, no delays
    tty.c_cc[VMIN] = 0;     // read doesn't block
    tty.c_cc[VTIME] = 5;    // 0.5 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    tty.c_cflag |= (CLOCAL | CREAD);   // ignore modem controls,
                                       // enable reading
    tty.c_cflag &= ~(PARENB | PARODD); // shut off parity
    tty.c_cflag |= parity;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr(fd, TCSANOW, &tty) != 0)
    {
        fprintf(stderr,"error %d from tcsetattr", errno);
        return -1;
    }
    return 0;
}

void set_blocking(int fd, int should_block)
{
    struct termios tty;
    memset(&tty, 0, sizeof tty);
    if (tcgetattr(fd, &tty) != 0)
    {
        fprintf(stderr,"error %d from tggetattr", errno);
        return;
    }

    tty.c_cc[VMIN] = should_block ? 1 : 0;
    tty.c_cc[VTIME] = 5; // 0.5 seconds read timeout

    if (tcsetattr(fd, TCSANOW, &tty) != 0)
        fprintf(stderr,"error %d setting term attributes", errno);
}

void *main_thread(void *d)
{
    char *portname = "/dev/ttyACM0";
    int fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0)
    {
        fprintf(stderr,"error %d opening %s: %s", errno, portname, strerror(errno));
        return NULL;
    }

    set_interface_attribs(fd, B115200, 0); // set speed to 115,200 bps, 8n1 (no parity)
    set_blocking(fd, 0);                   // set no blocking

    write(fd, "hello!\n", 7); // send 7 character greeting

    usleep((7 + 25) * 100); // sleep enough to transmit the 7 plus
                            // receive 25:  approx 100 uS per char transmit
    while(!event_queue.quit)
    {
        uint8_t type;
        int len;
        if(read(fd, &type, 1))
        {
            get_message_size(type, &len);
            char buf[len];
            usleep((len + 25) * 100);
            if(!read(fd, &buf, sizeof buf))
            {
                fprintf(stderr, "error %d serial %s: %s", errno, portname, strerror(errno));
            }
            printf("readed [%d/%ld] %s\n", len, sizeof(message), buf);
        }
    }

    /*************************************************************/


/*
    pc.baud(115200);
    pc.attach(&Rx_interrupt, Serial::RxIrq); // attach interrupt handler to receive data
    pc.attach(&Tx_interrupt, Serial::TxIrq); // attach interrupt handler to transmit data

    abort_btn.rise(&pressed);

    while (pc.readable())
        pc.getc();

    message msg = {.type = MSG_STARTUP, .data.startup.message = {'P', 'R', 'G', '-', 'L', 'A', 'B', '1', '0'}};
    uint8_t msg_buf[MESSAGE_SIZE];
    int msg_len;

    if (fill_message_buf(&msg, msg_buf, MESSAGE_SIZE, &msg_len))
    {
        for (int i = 0; i < msg_len; i++)
        {
            while (!pc.writable())
                ;
            pc.putc(msg_buf[i]);
        }
    }

    struct
    {
        uint16_t chunk_id;
        uint16_t nbr_tasks;
        uint16_t task_id;
        bool computing;
    } computation = {0, 0, 0, false};

    while (1)
    {
        if (rx_in != rx_out)
        {
            if (receive_message(msg_buf, MESSAGE_SIZE, &msg_len))
            {
                if (parse_message_buf(msg_buf, msg_len, &msg))
                {
                    switch (msg.type)
                    {
                    case MSG_GET_VERSION:
                        msg.type = MSG_VERSION;
                        msg.data.version.major = VERSION_MAJOR;
                        msg.data.version.minor = VERSION_MINOR;
                        msg.data.version.patch = VERSION_PATCH;
                        send_message(&msg, msg_buf, msg_len);
                        break;
                    case MSG_ABORT:
                        msg.type = MSG_OK;
                        send_message(&msg, msg_buf, msg_len);
                        computation.computing = false;
                        ticker_led.detach();
                        myled = 0;
                        break;
                    case MSG_COMPUTE:
                        computation.chunk_id = msg.data.compute.chunk_id;
                        computation.nbr_tasks = msg.data.compute.nbr_tasks;
                        computation.task_id = 0;
                        computation.computing = true;
                        ticker_led.attach(&flip, FLIP_PERIOD);
                        message_abort = false;
                        msg.type = MSG_OK;
                        send_message(&msg, msg_buf, msg_len);
                    default:
                        break;
                    }
                }
                else
                {
                    msg.type = MSG_ERROR;
                    send_message(&msg, msg_buf, msg_len);
                }
            }
        }
        else
        {
            if (computation.computing)
            {
                if (computation.task_id < computation.nbr_tasks && !message_abort)
                {
                    wait(0.5);
                    computation.task_id++;
                    msg.type = MSG_COMPUTE_DATA;
                    msg.data.compute_data.chunk_id = computation.chunk_id;
                    msg.data.compute_data.task_id = computation.task_id;
                    msg.data.compute_data.result = rand() % 255;
                    send_message(&msg, msg_buf, msg_len);
                }
                else
                {
                    computation.computing = false;
                    if (message_abort)
                        msg.type = MSG_ABORT;
                    else
                        msg.type = MSG_DONE;
                    send_message(&msg, msg_buf, msg_len);
                    ticker_led.detach();
                    myled = 0;
                }
            }
            else
                sleep();
        }
    }
    */
    pthread_exit(NULL);
}