#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main() {
	int pid, fd;
	int  *heappid = malloc(sizeof(int));;	
	char *addr = malloc(20 * sizeof(char));
	char *fdstr = malloc(2 * sizeof(char));
	char *heapaddr = malloc(20 * sizeof(char));
	fd = open("./Makefile");

	
	printf("env.EXEC=%s\n", getenv("EXEC"));
	printf("strcmp %d\n", strcmp(getenv("EXEC"), "P"));
	pid = fork();
			
	if (pid < 0) {
		perror("Fork");
		return 1;
	}
	if ((strcmp(getenv("EXEC"), "P") == 0 && pid != 0) || 
		(strcmp(getenv("EXEC"), "C") == 0 && pid == 0)) {
		char buf[3] = {'\0'};

		read(fd, buf, 2);

		*heappid = pid;

		sprintf(addr, "%ld", (long)&pid);	
		sprintf(fdstr, "%d", fd);	
		sprintf(heapaddr, "%ld", (long)heappid);	
		
		printf("content of first 2 char of Makefile is \n%s\n", buf);
		printf("stackaddr = (%p <- %ld), heapaddr = (%p <- %s)\n", &pid, &pid, heappid, heapaddr);
		printf("Try to depointer stac *stacpid=%d\n", pid);
		printf("Try to depointer heap *heappid=%d\n", *heappid);	

		//char *args[] = {"./hello", addr, heapaddr, fdstr};
		/* The list of arguments must be terminated by a NULL pointer, 
		   and, since these are variadic functions, this pointer must be cast (char *) NULL. */
		
		char *args[] = {"./hello", addr, heapaddr, fdstr, (char *)NULL};

		int ret = execve("./hello", args, NULL);
		if (ret < 0) {
			perror("execve");
		}
		exit(0);
	} else {
		printf("parent\n");
	}
	waitpid(pid, NULL, 0);	
	return 0;
}
