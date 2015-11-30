#include <stdio.h>
#include <unistd.h>

int main() {
	int *base = sbrk(0);
	
//	brk(base + 1);
	printf("base = %p\n", base);
	*base = 1;//seg here

	return 0;		
}
