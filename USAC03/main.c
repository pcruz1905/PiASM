#include "asm.h"

int main() {
	int value;
	char str[] ="89";
	int res = get_number(str, &value);
	printf ("%d : %d\n",res, value); //1 : 89
	char str2[] = "8--9";
	res = get_number(str2, &value);
	printf("%d : %d\n",res, value); //0:
	char str3[] = "-9";
	res = get_number(str3, &value);
	printf("%d : %d\n",res, value); //0:
	return 0;
}