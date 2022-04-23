#include<stdio.h>
#include<stdlib.h>

int main(){
    srand(time(NULL));
    int r = rand() % 10;
    int d = rand() % 10;
    printf("%d, %d\n", r, d);
    return 0;
}