#include<stdio.h>
#include "asm.h"
void print_buffer(int* buffer, int length, int tail, int head) {
    printf("Buffer: [ ");
    for (int i = 0; i < length; i++) {
        printf("%d ", buffer[i]);
    }
    printf("] | Tail: %d | Head: %d\n", tail, head);
}

int main() {
    int buffer[5] = {10, 20, 30, 40, 50};
    int length = 5;                      
    int tail = 0;                        
    int head = 3;                        
    int value;                           

    // Print the initial state of the buffer
    printf("Initial buffer state:\n");
    print_buffer(buffer, length, tail, head);

    // Dequeue a value
    if (dequeue_value(buffer, length, &tail, &head, &value)) {
        printf("Dequeued value: %d\n", value);
    } else {
        printf("Queue is empty\n");
    }

    // Print the state of the buffer after dequeue
    printf("Buffer state after dequeue:\n");
    print_buffer(buffer, length, tail, head);

    return 0;
}
