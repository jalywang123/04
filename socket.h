#ifndef SOCKET_H
#define SOCKET_H


#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>



void socketinit();
void senddata(char *sendbuf,int len);
int revdata(char *revbuf,int len);



























#endif // SOCKET_H

