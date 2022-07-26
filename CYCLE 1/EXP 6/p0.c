#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include "port.h"

int eat=0;
int think=0;
int wait=0;

int addrlen;
int sock;
struct sockaddr_in server;

void initialize(){

    sock=socket(AF_INET,SOCK_STREAM,0);
    if (sock<0){
        perror("Socket creation failed\n");
    }
    printf("Socket created successfully\n");

    addrlen=sizeof(server);
    
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=inet_addr("127.0.0.1");
    server.sin_port=htons(port);
    
    if (connect(sock,(struct sockaddr*)&server,(socklen_t)addrlen)<0){
        perror("Connection failed\n");
    }
    printf("Connection successful\n");

    while (1){
        char *msg;
        msg=(char *)malloc(sizeof(char)*10);
        send(sock,NULL,0,0);
        recv(sock,msg,10*sizeof(char),0);
        if (strcmp(msg,"COMPLETED")==0){
            exit(EXIT_SUCCESS);
        }
        printf("PHILLOSOPHER 0 STATUS: %s\n",msg);
        msg[0]='\0';
        fflush(stdout);
    }
    
}

void main(){
    initialize();

    close(sock);
}