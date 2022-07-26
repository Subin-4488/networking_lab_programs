#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

struct clientData{
    char username[50];
    char password[50];
    int clientfd;
}cd[10];

#define STOR "STOR"
#define RETR "RETR"
#define QUIT "QUIT"
#define OK "OK"
#define NOT_OK "NOT_OK"
#define port 8080

void printError(char error[20], int flag)
{
    printf("%s\n",error);
    if (flag)   exit(EXIT_FAILURE);
}

int addrlen=sizeof(struct sockaddr_in);