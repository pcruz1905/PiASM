#include <stdio.h>
#include "asm.h"

int main()
{
    int vec[] = {3, 5, 1, 7, 9};
    int length = sizeof(vec) / sizeof(vec[0]);
    int me;

    int result = median(vec, length, &me);

    if (result == 1)
    {
        printf("The median is: %d\n", me);
    }
    else
    {
        printf("Error calculating the median.\n");
    }

    return 0;
}
