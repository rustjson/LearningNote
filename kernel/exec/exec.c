#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main() {
	int pid, fd;
	int *heappid;
	heappid = malloc(sizeof(int));;	
	fd = open("./Makefile");

	pid = fork();
	*heappid = pid;

	char *addr = malloc(20 * sizeof(char));
	char *fdstr = malloc(2 * sizeof(char));
	char *heapaddr = malloc(20 * sizeof(char));
	
	sprintf(addr, "%ld", (long)&pid);	
	sprintf(fdstr, "%d", fd);	
	sprintf(heapaddr, "%ld\n", (long)heappid);	
			
	if (pid < 0) {
		perror("Fork");
		return 1;
	}
	if (pid == 0) {
		char buf[1000];

		read(fd, buf, 2);

		printf("content of Makefile is \n%s\n", buf);
		printf("stackaddr = (%p <- %ld), heapaddr = (%p <- %s)\n", &pid, &pid, heappid, heapaddr);

		char *args[] = {"./hello", addr, fdstr};
		//char *args[] = {"./hello", addr, heapaddr};

		int ret = execve("./hello", args, NULL);
		if (ret < 0) {
			perror("execve");
		}
	} else {
		printf("parent\n");
	}
	waitpid(pid, NULL, 0);	
	return 0;
}
