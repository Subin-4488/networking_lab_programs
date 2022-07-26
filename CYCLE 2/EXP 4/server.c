#include "head.h"

int sockfd;
struct sockaddr_in server, client;
int senderfd, receiverfd;
char msg[200];
struct queue q;

void sendAcknowledgement(int fd,int flag)
{
    if (flag)
    {
        send(fd, OK, sizeof(OK), 0);
        return;
    }
    else
    {
        send(fd, NOT_OK, sizeof(NOT_OK), 0);
        return;
    }
}

int cmpr(char m1[20], char m2[20])
{
    for (int i = 0; i < strlen(m2); i++)
    {
        if (m1[i] != m2[i])
            return 0;
    }
    return 1;
}

void *sender(void *num)
{
    struct mail m;
    while (1)
    {
        msg[0]='\0';
        recv(senderfd, msg, sizeof(char) * 200, 0);
        if (cmpr(msg, HELLO))
        {
            printf("Command received: %s\n", HELLO);
            sendAcknowledgement(senderfd,1);
            printf("Sending ack...\n");
            if (recv(senderfd, msg, sizeof(char) * 200, 0) && cmpr(msg, MAILFROM))
            {
                printf("Command received: %s\n",msg);
                strcpy(m.src, msg); // bind source
                sendAcknowledgement(senderfd,1);
                printf("Sending ack...\n");
                if (recv(senderfd, msg, sizeof(char) * 200, 0) && cmpr(msg, RECIPIENT))
                {
                    printf("Command received: %s\n",msg);
                    strcpy(m.recepient, msg); // bind recepient
                    sendAcknowledgement(senderfd,1);
                    printf("Sending ack...\n");
                    if (recv(senderfd, msg, sizeof(char) * 200, 0) && cmpr(msg, DATA))
                    {
                        printf("Command received: %s", msg);
                        sendAcknowledgement(senderfd,1);
                        printf("Sending ack...\n");
                        strcpy(m.msg, msg); // bind body
                        q.que[q.k++] = &m;
                    }
                    else
                    sendAcknowledgement(senderfd,0);
                }
                else
                sendAcknowledgement(senderfd,0);
            }
            else
            sendAcknowledgement(senderfd,0);
        }
        else
        sendAcknowledgement(senderfd,0);
    }
}

void *receiver()
{
    char msg[200];
    char temp[100];

    while (1)
    {
        receiverfd = accept(sockfd, (struct sockaddr *)&client, (socklen_t *)&addrlen);
        if (receiverfd < 0)
        {
            perror("Error accepting client\n");
            exit(EXIT_FAILURE);
        }
        recv(receiverfd, msg, sizeof(char) * 200, 0);
        if (cmpr(msg, HELLO))
        {
            printf("Command received: %s\n", HELLO);
            printf("Sending ack...\n");
            sendAcknowledgement(receiverfd,1);
            recv(receiverfd, msg, sizeof(char) * 200, 0);
            if (cmpr(msg, RECIPIENT))
            {
                printf("Command received: %s\n",msg);
                sendAcknowledgement(receiverfd,1);
                printf("Sending ack...\n");
                struct mail qtemp[10];  //default size alloted is for 10 msgs  
                int p=0;
                printf("Processing messages...\n");
                for (int i=0;i<q.k;i++){
                    if (!strcmp(q.que[i]->recepient,msg)){
                        strcpy(qtemp[p].msg,q.que[i]->msg);
                        strcpy(qtemp[p].src,q.que[i]->src);
                        strcpy(qtemp[p].recepient,q.que[i]->recepient);
                        p++;
                    }
                }
                send(receiverfd,&qtemp,sizeof(struct mail)*10,0);
                sleep(2);
                send(receiverfd,&p,sizeof(int),0);
                printf("Success\n");
            }
        }
    }
}

void initializeTcp()
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = port;

    if (bind(sockfd, (struct sockaddr *)&server, addrlen) < 0)
    {
        perror("Error while binding\n");
        exit(EXIT_FAILURE);
    }

    listen(sockfd, 4);
    printf("Listening...\n");
    senderfd = accept(sockfd, (struct sockaddr *)&client, (socklen_t *)&addrlen);
    if (senderfd < 0)
    {
        perror("Error accepting sender\n");
        exit(EXIT_FAILURE);
    }
    printf("sender connected\n");
}

void main()
{
    pthread_t t1, t2;
    printf("Initiazing SMTP server...\n");
    initializeTcp();

    pthread_create(&t1, NULL, sender, NULL);
    pthread_create(&t2, NULL, receiver, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}