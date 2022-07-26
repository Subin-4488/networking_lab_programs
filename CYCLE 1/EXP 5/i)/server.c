#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define port 8080

int sockfd,client1fd,client2fd;
struct sockaddr_in server,client1,client2;
int addrlen;
char *msg,*rev;

void initialize(){
    
    printf("Creating socket...\n");
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if (sockfd<0){
        perror("Socket creation failed\n");
    }
    else{
        printf("Socket created successfully\n");
    }

    server.sin_family=AF_INET;
    server.sin_addr.s_addr=INADDR_ANY;
    server.sin_port=htons(port);
    addrlen=sizeof(server);

    
    printf("Binding...\n");
    if (bind(sockfd,(struct sockaddr*)&server,addrlen)<0){
        perror("Bind failed...\n");
        exit(EXIT_FAILURE);
    }
    else{
        printf("Bind success...\nListening to incoming connections...\n");
    }

    listen(sockfd,2);

    client1fd=accept(sockfd,(struct sockaddr*)&client1,(socklen_t*)&addrlen);

    if (client1fd<0){
        perror("Accept failed...\n");
    }
    else{
        printf("Accept success...\n");
    }
}

void reverse(){
    msg=(char *)malloc(sizeof(char)*40);
    recv(client1fd,msg,sizeof(char)*40,0);
    printf("Received string: %s\n",msg);

    rev=(char *)malloc(sizeof(char)*strlen(msg));

    for (int i=0,j=strlen(msg)-1;i<strlen(msg),j>=0;i++,j--){
        rev[j]=msg[i];
    }

    printf("Reversed string: %s\n",rev);
}

void sendToClient2(){

    client2fd=accept(sockfd,(struct sockaddr*)&client2,(socklen_t*)&addrlen);
    if (client2fd<0){
        perror("Accepting client 2 failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Client 2 accepted...\n");
    printf("Sending data to client2...\n");

    if (send(client2fd,rev,sizeof(char)*strlen(rev),0)<0){
        perror("Data send failed...\n");
        exit(EXIT_FAILURE);
    }
    printf("Data send successfull!!...\n");
    exit(EXIT_SUCCESS);
}

void main(){
    
    printf("*********SERVER********");

    initialize();
    reverse();
    sendToClient2();
}