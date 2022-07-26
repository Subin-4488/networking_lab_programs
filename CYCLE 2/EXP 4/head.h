#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#define port 8050
#define HELLO "HELLO"
#define MAILFROM "MAILFROM: "
#define RECIPIENT   "RECEPIENT: "
#define DOMAIN "@gmail.com"
#define DATA "DATA: "
#define OK  "OK"
#define NOT_OK  "NOT_OK"

struct mail{
    char src[50];
    char recepient[50];
    char msg[100];
};

struct queue
{
    int k;
    struct mail *que[20];
};

int addrlen=sizeof(struct sockaddr_in);