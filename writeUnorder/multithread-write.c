#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdlib.h>



#define DST "./test.txt"
#define TNUM 1000

typedef struct _arg {
	int fd;
	int thread_id;	
}arg_t;

void *writeFile(void *_arg) {
	arg_t arg = *(arg_t *) _arg;
	

	write(arg.fd, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\n", 53);
	//printf("%d\n", arg.thread_id); 
}

int main() {
	int writeBytes = 0;
	pthread_t thread[TNUM];
	arg_t args[TNUM];
	int i = 0;

	int fd = open(DST, O_APPEND|O_RDWR|O_CREAT, S_IRWXU);
	if (fd == -1) {
		perror("open");
		return -1;
	}
	
	for (i = 0; i < TNUM; i++) {
		args[i].thread_id = i;
		args[i].fd = fd;
	}
	for (i = 0; i < TNUM; i++) {
		pthread_create(thread + i, NULL, writeFile , args + i);
	}
	for (i = 0; i < TNUM; i++) {
		pthread_join(thread[i], (void **)NULL);
	}
	close(fd);	
	return 0;
}
