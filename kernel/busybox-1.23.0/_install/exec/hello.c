#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {

	int *stacpid = NULL;
	int *heappid = NULL;
	
	printf("-----------output of hello------------\n");

	
	stacpid = (int *)atol(argv[1]);
	heappid = (int *)atol(argv[2]);
	
	printf("stackaddr = (%p <- %s), heapaddr = (%p <- %s)\n", stacpid, argv[1], heappid, argv[2]);

	*heappid = 1;
	*stacpid = 2;	
	printf("Try to depointer heap *heappid=%d\n", *heappid);	
	printf("Try to depointer stac *stacpid=%d\n", *stacpid);

	printf("fd=%s\n", argv[3]);

		char buf[2] = {};

		if (read(atol(argv[3]), buf, 2) < 0) {
			perror("read");
		}

		printf("content of Makefile is \n%s\n", buf);
	return 0;
}

