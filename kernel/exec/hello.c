#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {

	int *stacpid = NULL;
	int *heappid = NULL;
	
	printf("-----------------------\n");
	printf("%s\n", argv[2]);
	printf("fd=%s\n", argv[2]);
		char buf[1000] = {};

		if (read(atol(argv[2]), buf, 1000) < 0) {
			perror("read");
		}

		printf("content of Makefile is \n%s\n", buf);

	printf("stackaddr = (%p <- %s), heapaddr = (%p <- %s)\n", (int *)atol(argv[1]), argv[1], (long *)atol(argv[2]), argv[2]);
	
	stacpid = (int *)atol(argv[1]);
	heappid = (int *)atol(argv[2]);
	
	printf("stackaddr = %p heapaddr = %p\n", stacpid, heappid);
	
	printf("Try to depointer heap *heappid=%d\n", *heappid);	
	printf("Try to depointer stac *stacpid=%d\n", *stacpid);	
	return 0;
}

