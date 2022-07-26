#include <stdio.h>
#include <stdlib.h>

void main(){
    char *num="54";

    int a=strtol(num,NULL,10);

    printf("%d\n",a);
}