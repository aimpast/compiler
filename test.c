#include<stdio.h>

int main(){
	unsigned int i=0xABCDEF;
	volatile char test=(*((char*)&i)<<24);
	printf("%02x\n",test>>24);
	return 0;
}
