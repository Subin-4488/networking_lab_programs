#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define port 8080

struct sockaddr_in server;
int sockfd;

void initialize(){

    sockfd = socket(SOCK_DGRAM, AF_INET, 0);
    if (sockfd < 0)
    {
        perror("Socket creation failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Socket created\n");

    char ch;
    while (1){

        printf("Enter any key to request time to server:\n");
        scanf("%c",&ch);

        server.sin_family=AF_INET;
        server.sin_addr.s_addr=inet_addr("127.0.0.1");
        server.sin_port=htons(port);

        char msg[50];
        strcpy(msg,"What is the time now?");
        
        sendto(sockfd,msg,sizeof(char)*strlen(msg),0,(struct sockaddr*)&server,(socklen_t)sizeof(server)); 

        char retmsg[100]; int addrlen=sizeof(server);
        recvfrom(sockfd,retmsg,sizeof(char)*100,0,(struct sockaddr*)&server,(socklen_t*)&addrlen);

        printf("%s\n",retmsg);
    }

}

void main(){
    initialize();
}