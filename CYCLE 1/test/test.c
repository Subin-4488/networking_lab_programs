#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void* fun1(void* a){

    int num= * (int *)a;

    while (1){
        printf("NUM: %d\n",num++);
        sleep(1);
    }

}

void* fun2(){
    char msg[20];

    while (1){
        printf("Enter any msg:\n");
        scanf("%s",msg);
        printf("Entered: %s\n",msg);
    }
}

void main() {

    pthread_t t1,t2;
    int a=10;
    
    pthread_create(&t1,NULL,fun1,(void *)&a);
    pthread_create(&t2,NULL,fun2,NULL);

    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
}