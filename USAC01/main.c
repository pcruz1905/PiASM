#include<stdio.h>
#include "asm.h"

int main() {
    char str[] = "TEMP&unit:celsius&value:20#HUM&unit:percentage&value:80";
    char token[] = "TEMP";
    char unit[20];
    int value;

    int res = extract_data(str, token, unit, &value);
    printf("%d:%s,%d\n", res, unit, value); 

    return 0;
}
