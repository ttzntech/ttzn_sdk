/*
 * Created on Sat Aug 10 2024
 *
 * Author: Joonchen Liau
 * Email: liaojuncheng123@foxmail.com
 *
 * Copyright (c) 2024 Tiante Intelligent Technology
 */

#include "ttzn_sdk/utils.hpp"


/**
 * @brief uart setup function
 * 
 * @param fd the device file descriptor corresponding to the uard
 * @param baude baude rate
 * @param c_flow control flow
 * @param bits data bit num
 * @param parity parity mode
 * @param stop stop bit
 * @return int 0 mean success other mean false
 */
int uart_set(int fd, uint64_t baude, int c_flow, int bits, char parity, int stop)
{
    
    struct termios options;
    
    /* get terminal attribute */
    if(tcgetattr(fd, &options) < 0)
    {
        perror("tcgetattr error");
        return -1;
    }
 
    /* set baude rate*/
    switch(baude)
    {
        case 4800:
            cfsetispeed(&options, B4800);
            cfsetospeed(&options, B4800);
            break;
        case 9600:
            cfsetispeed(&options, B9600);
            cfsetospeed(&options, B9600);
            break;
        case 19200:
            cfsetispeed(&options, B19200);
            cfsetospeed(&options, B19200);
            break;
        case 38400:
            cfsetispeed(&options, B38400);
            cfsetospeed(&options, B38400);
            break;
        case 115200:
            cfsetispeed(&options, B115200);
            cfsetospeed(&options, B115200);
            break;
        default:
            fprintf(stderr,"Unkown baude!\n");
            return -1;
    }
 
    /* set control mode */
    options.c_cflag |= CLOCAL;  /* Ensure that the program does not occupy the serial port */
    options.c_cflag |= CREAD;   /* Ensure that the program can read data from serial port */
 
    /* set data flow control method */
    switch(c_flow)
    {
        case 0: /* no flow control */
            options.c_cflag &= ~CRTSCTS;
            break;
        case 1: /* hardware flow control */
            options.c_cflag |= CRTSCTS;
            break;
        case 2: /* software flow control */
            options.c_cflag |= IXON|IXOFF|IXANY;
            break;
        default:
            fprintf(stderr,"Unkown c_flow!\n");
            return -1;
    }
 
    /* set data bit num */
    switch(bits)
    {
        case 5:
            options.c_cflag &= ~CSIZE;
            options.c_cflag |= CS5;
            break;
        case 6:
            options.c_cflag &= ~CSIZE;
            options.c_cflag |= CS6;
            break;
        case 7:
            options.c_cflag &= ~CSIZE;
            options.c_cflag |= CS7;
            break;
        case 8:
            options.c_cflag &= ~CSIZE;
            options.c_cflag |= CS8;
            break;
        default:
            fprintf(stderr,"Unkown bits!\n");
            return -1;
    }
 
    /* set parity */
    switch(parity)
    {
        /* withoud parity */
        case 'n':
        case 'N':
            options.c_cflag &= ~PARENB; /* disable parity bit */
            break;
        /* stop bit num set to 2 */
        case 's':
        case 'S':
            options.c_cflag &= ~PARENB; /* disabe parity bit */
            options.c_cflag &= ~CSTOPB; /* set stop bit num to 2 */
            break;
        /* set odd parity */
        case 'o':
        case 'O':
            options.c_cflag |= PARENB; /* PARENB: If set, Generate parity bit and perform parity check */
            options.c_cflag |= PARODD; /* PARODD: If set, it is odd check, otherwise it is even check */
            options.c_cflag |= INPCK; /* INPCK: Enable parity check */
            options.c_cflag |= ISTRIP; /* ISTRIP: If set, the valid input number is stripped of 7 bytes, otherwise all 8 bits are retained */
            break;
        /* set even parity */
        case 'e':
        case 'E':
            options.c_cflag |= PARENB; /* PARENB: If set, Generate parity bit and perform parity check */
            options.c_cflag &= ~PARODD; /* PARODD: If set, it is odd check, otherwise it is even check */
            options.c_cflag |= INPCK; /* INPCK: Enable parity check */
            options.c_cflag |= ISTRIP; /* ISTRIP: If set, the valid input number is stripped of 7 bytes, otherwise all 8 bits are retained */
            break;
        default:
            fprintf(stderr,"Unkown parity!\n");
            return -1;
    }
 
    /* set stop bit num */
    switch(stop)
    {
        case 1:
            options.c_cflag &= ~CSTOPB;
            break;
        case 2:
            options.c_cflag |= CSTOPB;
            break;
        default:
            fprintf(stderr,"Unkown stop!\n");
            return -1;
    }
 
    /* set to local recv mode */ 
    options.c_cflag |= (CLOCAL | CREAD);

    /* set origin input mode */ 
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    /* set origin output mode */ 
    options.c_oflag &= ~OPOST;

    /* flush serial port cache */ 
    tcflush(fd, TCIFLUSH);
 
    /* activate config */
    if(tcsetattr(fd, TCSANOW, &options) < 0)
    {
        perror("tcsetattr failed");
        return -1;
    }
 
    return 0;
}