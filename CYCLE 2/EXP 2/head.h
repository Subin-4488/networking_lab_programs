#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#define port 8080

struct node{
    char id;
    int edge;
};

struct head{
    char name;
    int neighbours;
    struct node ptr[10];
};

struct graph{
    struct head arr[20];
    int n;
};


struct cost
{
    char id;
    int cost;
}c[10];