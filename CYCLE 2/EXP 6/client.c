#include "head.h"

int sockfd;
struct sockaddr_in server;

void initialize()
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        printError("Socket creation failed", 1);

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(port);
    server.sin_family = AF_INET;

    if (connect(sockfd, (struct sockaddr *)&server, (socklen_t)addrlen) < 0)
        printError("Connect failed", 1);
}

void retrieve()
{
    char msg[50];
    char file[50];

    printf("Enter filename:\n"); // file name to look for inside directory "FILES"
    msg[0] = '\0';
    scanf("%s", msg);

    strcpy(file, msg);
    sleep(1);
    send(sockfd, file, sizeof(char) * 20, 0);
    printf("Downloading...\n");
    sleep(1);
    int flag = 0;
    recv(sockfd, msg, sizeof(char) * 20, 0);
    printf("SERVER PROCESS: %s\n\n", msg);
    if (!strcmp(msg, NOT_OK))
    {
        printf("File doesnot exist in the server...\n");
        printf("ACKNOWLEGEMENT: %s\n", msg);
        return;
    }
    while (recv(sockfd, msg, sizeof(char) * 20, 0) > 0)
    {
        if (!strcmp(msg, NOT_OK))
        {
            break;
        }
        flag = 1;
        printf("%s ", msg);
        fflush(stdout);
    }
    if (!flag)
    {
        printf("File doesnot exist in the server...\n");
        printf("ACKNOWLEGEMENT: %s\n", msg);
    }
}

void main()
{
    printf("Connecting to server...\n");
    sleep(1);
    initialize();
    printf("Connected to server\n");

    while (1)
    {
        int option;
        printf("Enter the OPTION: \n1:DOWNLOAD FILE\n");
        scanf("%d", &option);

        switch (option)
        {
        case 1:
            retrieve();
            break;
        default:
            printf("Enter valid option\n");
            break;
        }
    }
}