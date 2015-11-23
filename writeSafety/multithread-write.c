#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdlib.h>



#define DST "./test.txt"
#define TNUM 1000

typedef struct _arg {
	FILE *file;
	int thread_id;	
}arg_t;

void *writeFile(void *_arg) {
	arg_t arg = *(arg_t *) _arg;
	

	fprintf(arg.file, "%s%d%s\n", "abcdefghijklmnopqrstuvwxyz", arg.thread_id, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	//printf("%d\n", arg.thread_id); 
}

int main() {
	int writeBytes = 0;
	pthread_t thread[TNUM];
	arg_t args[TNUM];
	int i = 0;

	FILE *fd = fopen(DST, "a+");
	if (fd == NULL) {
		perror("open");
		return -1;
	}
	
	for (i = 0; i < TNUM; i++) {
		args[i].thread_id = i;
		args[i].file = fd;
	}
	for (i = 0; i < TNUM; i++) {
		pthread_create(thread + i, NULL, writeFile , args + i);
	}
	for (i = 0; i < TNUM; i++) {
		pthread_join(thread[i], (void **)NULL);
	}
	fclose(fd);	
	return 0;
}
