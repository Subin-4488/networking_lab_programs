#include "head.h"

struct sockaddr_in server, client;
int addrlen = 0;
int sockfd = 0, clientfd = 0;

void initialize()
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Socket creation failed!!\n");
        exit(EXIT_FAILURE);
    }
    printf("Socket created successully\n");

    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    addrlen = sizeof(server);
    if (bind(sockfd, (struct sockaddr *)&server, (socklen_t)addrlen) < 0)
    {
        perror("Bind failed!!\n");
        exit(EXIT_FAILURE);
    }
    printf("Bind successful\n");

    listen(sockfd, 2);
    printf("listening...\n");

    clientfd = accept(sockfd, (struct sockaddr *)&client, (socklen_t *)&addrlen);
    if (clientfd < 0)
    {
        perror("Accept failed!!\n");
        exit(EXIT_FAILURE);
    }
    printf("Client accepted\n");
}

int getItemIndex(char id, int n)
{
    for (int i = 0; i < n; i++)
    {
        if (c[i].id == id)
        {
            return i;
        }
    }
}

/*
Net edge weight inside a loop must be > 0  
*/
void bellMannFord(struct graph g)
{
    for (int i = 0; i < g.n; i++)
    {
        c[i].cost = __INT_MAX__;
        c[i].id = g.arr[i].name;
    }
    c[0].cost = 0; // root node

    for (int k = 0; k < g.n - 1; k++)
    {
        for (int i = 0; i < g.n; i++)
        {
            for (int j = 0; j < g.arr[i].neighbours; j++)
            {
                if (g.arr[i].ptr[j].edge + c[getItemIndex(g.arr[i].name, g.n)].cost < c[getItemIndex(g.arr[i].ptr[j].id, g.n)].cost)
                {
                    c[getItemIndex(g.arr[i].ptr[j].id, g.n)].cost = g.arr[i].ptr[j].edge + c[getItemIndex(g.arr[i].name, g.n)].cost;
                }
            }
        }
    }
}

void main()
{
    initialize();

    struct graph gres;
    recv(clientfd, (struct graph *)&gres, sizeof(struct graph), 0);
    // debug(gres);
    bellMannFord(gres);
    printf("Running bellmanford routing algorithm...\n");
    printf("Sending result...\n");
    send(clientfd,&c,sizeof(struct cost)*10,0);

}   