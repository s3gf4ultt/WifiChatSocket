#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>
#include <linux/in.h>
#include <sys/socket.h>

// server address
#define SV_ADDR "192.168.0.5"
#define SV_PORT 50000

// exchange data length for communication between client and server
#define EXCLEN 100

int svinit();
int svlisten();

#endif