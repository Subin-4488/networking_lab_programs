#include "head.h"

struct sockaddr_in server;
int sockfd;

void initialize()
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = port;
    if (connect(sockfd, (struct sockaddr *)&server, addrlen) < 0)
    {
        perror("Connect failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Connected to server\n");
}

int cmprDomain(char m1[100], char m2[100])
{
    int i, j;
    for (i = strlen(m1) - 1, j = strlen(m2) - 1; j >= 0; i--, j--)
    {
        if (m1[i] != m2[j])
            return 0;
    }
    return 1;
}

int receiveAck()
{
    char temp[10];
    recv(sockfd, temp, sizeof(char) * 9, 0);    printf("%s\n",temp);
    if (!strcmp(temp, OK))
        return 1;
    else
        return 0;
}

void *sendMessage()
{
    char msg[200];
    char temp[200];
    int t;
    char t1;

    while (1)
    {
        msg[0]='\0';
        printf("Enter 1 to start sending message:\n");
        scanf(" %d", &t);
        if (t == 1)
        {
            send(sockfd, HELLO, sizeof(HELLO), 0);
            if (receiveAck())
            {
                printf("Acknowledgement received\n");
                printf("Enter your email address:\n");
                scanf(" %s", msg);
                while (!cmprDomain(msg, DOMAIN))
                {
                    printf("Enter proper domain\n");
                    printf("Enter your email address:\n");
                    scanf(" %s", msg);
                }
                strcpy(temp,MAILFROM); strcat(temp,msg); strcpy(msg,temp);
                send(sockfd, msg, sizeof(msg), 0);
                if (receiveAck())
                {
                    printf("Acknowledgement received\n");
                    printf("Enter recepient email address:\n");
                    scanf(" %s", msg);
                    while (!cmprDomain(msg, DOMAIN))
                    {
                        printf("Enter proper domain:\n");
                        printf("Enter recepient email address:\n");
                        scanf(" %s", msg);
                    }
                    strcpy(temp,RECIPIENT); strcat(temp,msg); strcpy(msg,temp);
                    send(sockfd, msg, sizeof(msg), 0);  
                    if (receiveAck())
                    {
                        printf("Acknowledgement received\n");
                        printf("Enter message body:\n");    scanf("%c",&t1);
                        fgets(msg, sizeof(char) * 198, stdin);
                        strcpy(temp,DATA); strcat(temp,msg); strcpy(msg,temp);
                        send(sockfd, msg, sizeof(msg), 0);
                        printf("Message sent successfully\n");
                    }
                    else
                    printf("Failed\n");
                }
            }
            else
            printf("Failed\n");
        }
    }
}

void main()
{
    initialize();
    pthread_t t;
    pthread_create(&t, NULL, sendMessage, NULL);
    pthread_join(t, NULL);
}