#include <stdio.h>
#include "asm.h"

int get_n_element(int* buffer, int length, int* tail, int* head);

int main() {
    int buffer[10] = { 0 };
    int tail = 2;
    int head = 7;
    int length = 10;

    int num_elements = get_n_element(buffer, length, &tail, &head);
    printf("Number of elements in the circular buffer: %d\n", num_elements);

    return 0;
}