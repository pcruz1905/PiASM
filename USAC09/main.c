    #include <stdio.h>
    #include "asm.h"

    int main(void) {
        int vec[10] = {1,3,9,4,5,23,12,8,64,7};
        printf("Vector before sorting:\n");
        for (int i=0;i<10;i++) {
            printf("%d: %d\n", i+1,vec[i]);
        }

        int res = sort_array(vec, 0, 1);
        if (res==1) {
            printf("Vector after sorting in ascending order:\n");
            for (int i=0;i<10;i++) {
                printf("%d: %d\n", i+1,vec[i]);
            }
        }
        else {printf("Sorting was not successful\n");}
        
        res = sort_array(vec, 10, 1);
        if (res==1) {
            printf("Vector after sorting in ascending order:\n");
            for (int i=0;i<10;i++) {
                printf("%d: %d\n", i+1,vec[i]);
            }
        }
        else {printf("Sorting was not successful\n");}

        res = sort_array(vec, 10, 0);
        if (res==1) {
            printf("Vector after sorting in descending order:\n");
            for (int i=0;i<10;i++) {
                printf("%d: %d\n", i+1,vec[i]);
        }
        }
        else {printf("Sorting was not successful\n");}


        

        return 0;
    }
