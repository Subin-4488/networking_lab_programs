#include "head.h"

struct graph g;
struct sockaddr_in server;
int sockfd;

void collectGraph(){
    printf("***ENTER GRAPH INFORMATION****\n");

    printf("Enter total no of nodes:\n");
    scanf("%d",&g.n);

    printf("Enter ID of each node(ID must be a character!!):\n");
    for (int i=0;i<g.n;i++){
        scanf(" %c",&g.arr[i].name);            //SKIP NEWLINE
    }

    for (int i=0;i<g.n;i++){
        printf("Enter no of direct neighbours of ROUTER %c: ",g.arr[i].name);
        scanf(" %d",&g.arr[i].neighbours); 
        printf("\nEnter ID of each connected neighbour of %c and weight of the edge: \n",g.arr[i].name);

        for (int k=0;k< (g.arr[i].neighbours);k++){
            scanf(" %c",&g.arr[i].ptr[k].id);
            scanf("%d",&g.arr[i].ptr[k].edge);
        }
    }
}

void initialize(){
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if (sockfd<0){
        perror("Socket creation failed!!\n");
        exit(EXIT_FAILURE);
    }
    printf("Socket created successully\n");

    server.sin_addr.s_addr=inet_addr("127.0.0.1");
    server.sin_family=AF_INET;
    server.sin_port=htons(port);

    if (connect(sockfd,(struct sockaddr*)&server,(socklen_t)sizeof(server))<0){
        perror("Connection failed!!\n");
        exit(EXIT_FAILURE);
    }
    printf("Connection successfull\n");
}

void debug(struct graph gm){
    printf("Entered\n");
    for (int i=0;i<gm.n;i++){
        printf("%c->",gm.arr[i].name);

        for (int j=0;j<gm.arr[i].neighbours;j++){
            printf("%c->",gm.arr[i].ptr[j].id);
        }
        printf("\n");
    }
}

void main(void){
    collectGraph();
    initialize();

    send(sockfd,&g,sizeof(struct graph),0);
    struct cost res[10];
    recv(sockfd,(struct cost*)&res,sizeof(struct cost)*10,0);

    printf("***RESULT OBTAINED***\n");
    for (int i=0;i<g.n;i++){
        printf("%c ",res[i].id);
    }
    printf("\n");
    for (int i=0;i<g.n;i++){
        printf("%d ",res[i].cost);
    }
    printf("\n");
}       