#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {

	int *stacpid = NULL;
	int *heappid = NULL;
	
	printf("-----------------------\n");
	printf("%s\n", argv[2]);
	printf("stackaddr = (%p <- %s), heapaddr = (%p <- %s)\n", (int *)atol(argv[1]), argv[1], (long *)atol(argv[2]), argv[2]);
	
	stacpid = (int *)atol(argv[1]);
	heappid = (int *)atol(argv[2]);
	
	printf("stackaddr = %p heapaddr = %p\n", stacpid, heappid);
	
	printf("Try to depointer *pid=%d\n", *stacpid);	
	return 0;
}

