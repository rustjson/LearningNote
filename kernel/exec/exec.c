#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main() {
	int pid;
	int *heappid;
	heappid = malloc(sizeof(int));;	

	pid = fork();
	*heappid = pid;

	char *addr = malloc(20 * sizeof(char));
	char *heapaddr = malloc(20 * sizeof(char));
	
	sprintf(addr, "%ld", (long)&pid);	
	sprintf(heapaddr, "%ld\n", (long)heappid);	
			
	if (pid < 0) {
		perror("Fork");
		return 1;
	}
	if (pid > 0) {
		printf("stackaddr = (%p <- %ld), heapaddr = (%p <- %s)\n", &pid, &pid, (int *)atol(heapaddr), heapaddr);

		char *args[] = {"./hello", addr, heapaddr};

		execve("./hello", args, NULL);
	} else {
		//printf("parent\n");
	}
	waitpid(pid, NULL, 0);	
	return 0;
}
