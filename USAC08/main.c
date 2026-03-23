#include<stdio.h>
#include "asm.h"
int main() {
    int buffer[5] = {0, 2, 3, 4, 5};  
    int array[3];  
    int tail = 4;  
    int head = 3;
    int n = 3;  

    if (move_n_to_array(buffer, 5, &tail, &head, n, array)) {
        printf("Move successful! Array: ");
        for (int i = 0; i < n; i++) {
            printf("%d ", array[i]);
        }
        printf("\nNew tail index: %d\n", tail);
    } else {
        printf("Not enough elements to move.\n");
    }

    return 0;
}
