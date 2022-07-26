#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>

#define port 8080

int sockfd;
struct sockaddr_in server;
int addrlen;
char *msg;

void initialize(){
    
    printf("Creating socket...\n");
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if (sockfd<0){
        perror("Socket creation failed\n");
        exit(EXIT_FAILURE);
    }
    else{
        printf("Socket created successfully\n");
    }

    server.sin_family=AF_INET;
    server.sin_addr.s_addr=inet_addr("127.0.0.1");
    server.sin_port=htons(port);

    addrlen=sizeof(server);

    printf("Connecting...\n");
    if (connect(sockfd,(const struct sockaddr*)&server,addrlen)<0){
        printf("Connect failed...\n");
        exit(EXIT_FAILURE);
    }
    
    printf("Connect success...\n");
}

void receiveData(){
    msg=(char *)malloc(sizeof(char)*40);

    printf("Receiving data...\n");
    if (recv(sockfd,msg,sizeof(char)*40,0)<0){
        perror("Receive failed...\n");
        exit(EXIT_FAILURE);
    }

    printf("Received message: %s\n",msg);
}

void main(){

    printf("*********CLIENT 2********");

    initialize();
    receiveData();
}