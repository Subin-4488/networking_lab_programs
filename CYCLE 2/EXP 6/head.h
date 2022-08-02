#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

#define RETR "RETR"
#define OK "OK"
#define NOT_OK "NOT_OK"
#define port 8060

void printError(char error[20], int flag)
{
    printf("%s\n",error);
    if (flag)   exit(EXIT_FAILURE);
}

int addrlen=sizeof(struct sockaddr_in);