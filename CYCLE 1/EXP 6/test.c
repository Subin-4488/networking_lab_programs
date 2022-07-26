#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
struct class{
    pthread_t t;
    int id;
}*c;

void* display(void * a){

    struct class *arg= (struct class*)a;

    while (1){
        printf("THREAD: %ld\t%d\n",arg->t,arg->id);
        sleep(1);
    }

}

void Do(){
    int a=0; 
    while (1){
        printf("a\n");
        sleep(2);
        printf("b\n");
        sleep(2);
    }
}

void main(){
    // c=(struct class*)malloc(sizeof(struct class)*3);

    // for (int i=0;i<3;i++){
    //     c[i].id=i;
    // }

    // for (int i=0;i<3;i++){
    //     pthread_create(&c[i].t,NULL,(void *)display,&c[i]);
    // }

    // for (int i=0;i<3;i++){
    //     pthread_join(c[i].t,NULL);
    // }
    Do();
}

/*


    

*/