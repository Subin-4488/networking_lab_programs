#include "head.h"

int sockfd,c1,c2;
struct sockaddr_in server,client1,client2;

pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;

char **queue; //circular queue
int size=10;	int f=-1,r=-1;

void initialize()
{
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if (sockfd<0)
	{
		perror("socket creation failed\n");
		exit(EXIT_FAILURE);
	}
	printf("socket created successfully\n");
	
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_family=AF_INET;
	server.sin_port=htons(port);
	
	if (bind(sockfd,(struct sockaddr*)&server,(socklen_t)addrlen)<0)
	{
		perror("Bind failed\n");
		exit(EXIT_FAILURE);
	}
	printf("bind successfull\n");
	
	listen(sockfd,2);
	
	c1=accept(sockfd,(struct sockaddr*)&client1,(socklen_t*)&addrlen);
	if (c1<0)
	{
		perror("sender accept failed\n");
		exit(EXIT_FAILURE);
	}
	printf("sender client accepted\n");
	
	c2=accept(sockfd,(struct sockaddr*)&client2,(socklen_t*)&addrlen);
	if (c2<0)
	{
		perror("receiver accept failed\n");
		exit(EXIT_FAILURE);
	}
	printf("receiver client accepted\n");
	
}

void *receive()
{
	char *msg;
	while (1)
	{
		if (f==-1 && r==-1)
		{
			pthread_mutex_lock(&lock);
			f=r=0;
			pthread_mutex_unlock(&lock);
			recv(c1, msg, sizeof(char)*50, 0);
			printf("RECEIVED FROM SENDER: %s\n",msg);
           	strcpy(queue[r],msg);		//add to queue
		}
		else if((r+1)%size==f)  //full
		{
			printf("OVERFLOW!!\n");
			sleep(1);
		}
		else
		{
			pthread_mutex_lock(&lock);
			r=(r+1)%size;
			pthread_mutex_unlock(&lock);
			recv(c1, msg, sizeof(char)*50, 0);
			printf("RECEIVED FROM SENDER: %s\n",msg);
           		queue[r]=msg;		//add to queue
		}
	}
}

void *forward()
{
	while (1)
	{
		if (f!=-1 && r!=-1)
		{
			if (f==r)
			{
				send(c2,queue[f],sizeof(char)*50,0);
				pthread_mutex_lock(&lock);
				f=r=-1;
				pthread_mutex_unlock(&lock);
			}
			else
			{
				send(c2,queue[f],sizeof(char)*50,0);
				pthread_mutex_lock(&lock);
				f=(f+1)%size;
				pthread_mutex_unlock(&lock);
			}
		}
		sleep(3);
	}
}

void main()
{
	initialize();
	
	queue=(char **)malloc(sizeof(char*)*size);
	for (int i=0;i<size;i++)
	{
		queue[i]=(char *)malloc(sizeof(char)*50);
	}
	
	pthread_t t1,t2;
	
	pthread_create(&t1,NULL,receive,NULL);
	sleep(2);
	pthread_create(&t2,NULL,forward,NULL);
	
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
}