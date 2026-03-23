#include "asm.h"

int main() {
	int value = 26; // 0 b11010
	char bits[5];
	int res = get_number_binary(value, bits);
	printf ("% d : %d , %d , %d , %d , % d \n" ,res , bits [4] , bits [3] , bits [2] , bits [1] , bits[0]) ; // 1: 1 ,1 ,0 ,1 ,0
	return 0;
}