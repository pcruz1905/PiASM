#include <stdio.h>
#include "asm.h"

int buffer[10]={1,2,3,4,5,6,7};
int* ptr_buffer=buffer;
int length=10;
int* tail=&buffer[9];
int* head=&buffer[0];
int value=43;

int main(void){
    printf("Hello world\n");
    
    printf("%d\n", *tail);
    printf("%p\n%p\n",head,tail);
    printf("%p\n", ptr_buffer);
    for(int i=0; i<length;i++) {
        printf("%d\n",buffer[i]);
    }
    int res=enque_value(ptr_buffer, length, tail, head, value);
    for(int i=0; i<length;i++) {
        printf("%d\n",buffer[i]);
    }
    printf("Output: %d\n",res);
    
    return 0;
}