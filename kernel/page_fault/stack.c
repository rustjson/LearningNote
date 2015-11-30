#include <stdio.h>

int main() {	
	int a = 1;
	int b = 2;
	int c = 2;
	int d = 2;
	int i = 0;

	printf("c = %p\n", &a);	
	//*(&a + i);
	for (i = 0; i < 4000; i++) {
		printf("i = %d,c = %d\t", i, *(&a+i));
		if (i % 5 == 0) printf("\n");
	}	
	return 0;
}
