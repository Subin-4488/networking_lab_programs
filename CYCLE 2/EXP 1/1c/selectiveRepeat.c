#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <time.h>
#include "head.h"

int frames[10];
int totalFrames = 10;
int resend=1;

int sockfd;
struct sockaddr_in server;
int addrlen;

pthread_t t;

clock_t clockStart, clockEnd;
double timeTaken;

void initialize()
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Socket creation failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Socket created successfully\n");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(port);
    addrlen = sizeof(server);

    if (connect(sockfd, (struct sockaddr *)&server, (socklen_t)addrlen) < 0)
    {
        perror("Connection to server failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Connection to server success\n");
}

void sendEntireWindow(int start, int end)
{
    char *msg = (char *)malloc(sizeof(char) * 100);
    
    for (int p = start; p <= end; p++)
    {
        msg[0]='\0';
        sprintf(msg, "%d", frames[p]);
        printf("Sending frame: %s\n", msg);
        send(sockfd, msg, sizeof(char) * strlen(msg), 0);
        sleep(1);
    }
}

int acknowledge(){
    resend=-1;
    char *retmsg = (char *)malloc(sizeof(char) * 100);
    recv(sockfd, retmsg, sizeof(char) * 100, 0);
    if (retmsg[0]=='N'){
        printf("Message from server: %s\n",retmsg);
        resend=retmsg[strlen(retmsg)-1]-'0';
        return 0;
    }
    else{
        printf("Message from server: %s\n",retmsg);
        return 1;
    }
}

void *selectiveRepeat(void *ptr)
{
    int windowSize = *(int *)ptr;

    char *msg = (char *)malloc(sizeof(char) * 100);
    char *retmsg = (char *)malloc(sizeof(char) * 100);

    sprintf(msg, "%d", windowSize);
    send(sockfd, msg, sizeof(char) * strlen(msg), 0);
    sleep(1);

    int i = 0;
    int flag = 0;
    int k = 0;
    while (1)
    {
        msg[0] = '\0';
        if (!flag){
            flag=1;
            sendEntireWindow(0,windowSize-1);
            i+=windowSize;
            while (!acknowledge()){
                for (int l=k;l<i;l++)
                {
                    if (frames[l]==resend){
                        sendEntireWindow(l,l);
                        break;
                    }
                }
            }
            k++;
        }
        else{
            sendEntireWindow(i,i);
            while (!acknowledge()){
                printf("RESEND********: %d\n",resend);
                for (int l=k;l<i;l++)
                {
                    printf("%d\t%d\n",frames[l],resend);
                    if (frames[l]==resend){
                        sendEntireWindow(l,l);
                        break;
                    }
                }
            }
            k++;    i++;
        }
        
        if (i == 10)
            break;
    }
}

void main()
{
    initialize();

    int windowSize;
    printf("Enter the sender window size:\n");
    scanf("%d", &windowSize);

    printf("Enter total no of frames:\n");
    scanf("%d", &totalFrames);

    int j = 0;
    for (int i = 0; i < totalFrames; i++)
    {
        if (j == windowSize)
        {
            j = 0;
        }
        frames[i] = j++;
    }

    if (windowSize > totalFrames)
    {
        printf("Error\n");
        exit(EXIT_FAILURE);
    }

    pthread_create(&t, NULL, selectiveRepeat, &windowSize);
    pthread_join(t, NULL);
}
