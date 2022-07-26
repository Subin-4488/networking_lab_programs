#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define port 8060

static int sockfd;
int clientfd[100]={-1};
struct sockaddr_in server;
static int clientCount=0;

pthread_mutex_t lock =PTHREAD_MUTEX_INITIALIZER;

void* readMessage(void * arg){

    int clientid=*(int *)arg;
    
    while (1){
        char *msg=(char *)malloc(sizeof(char)*100);

        recv(clientid,msg,sizeof(char)*100,0);

        if (strlen(msg)>0){
            printf("%s\n",msg); 
            for (int i=0;i<clientCount;i++){

                fflush(stdin); fflush(stdout);
                send(clientfd[i],msg,sizeof(char)*strlen(msg),0);

            }
            msg[0]='\0'; //make string length zero
        }
        strcpy(msg,"");
    }
}

void * listenAccept(void *arg){
     while (1){
        clientfd[clientCount]=accept(sockfd,(struct sockaddr*)NULL,(socklen_t *)NULL);
        if (clientfd[clientCount]<0){
            printf("Accept failed!!\n");
        }
        else{
            printf("Accept success\n");
            pthread_t t;
            int c=clientfd[clientCount];
            pthread_create(&t,NULL,readMessage,(void *)&c);
            
            //pthread_join(t,NULL);

            pthread_mutex_lock(&lock);
            clientCount++;
            pthread_mutex_unlock(&lock);

        }
    }
}

void initialize(){
    
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if (sockfd==-1){
        perror("Socket creation failed!!\n");
        return;
    }
    else{
        printf("Socket created successfully\n");
    }

    server.sin_family=AF_INET;
    server.sin_addr.s_addr=INADDR_ANY;
    server.sin_port=htons(port);

    if (bind(sockfd,(struct sockaddr*)&server,(socklen_t)sizeof(server))<0){
        perror("Bind failed!!\n");
        return;
    }
    printf("Bind successfull\n");

    listen(sockfd,10);
}

void main(void){
    initialize();

    pthread_t t1;
    pthread_create(&t1,NULL,listenAccept,NULL);   
    pthread_join(t1,NULL);

    close(sockfd);
}