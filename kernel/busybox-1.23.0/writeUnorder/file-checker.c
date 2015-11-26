#include "write.h"

int main() {
	int fd, readBytes, i, j;
	char buf[BUF_SIZE];

	fd = open(DST, O_RDONLY);
	
	if (fd == -1) {
		perror("Open");
		return 1;
	}
	for (i = 0; i < COUNT * TNUM; i++) {
		readBytes = read(fd, buf, BUF_SIZE);
		if (readBytes == BUF_SIZE) {
			for(j = 1; j < 1024; j++) {
				if (buf[j] != buf[0] ){
					printf("Found error at i=%d, j=%d, char=%c, buf[0]=%c\n", i, j, buf[j], buf[0]);
					return 0;
				}
			}
			printf("Block %d, size=%d check pass with char=%c\n", i, i * BUF_SIZE, buf[0]);
		} else {
			printf("File end with %d bytes\n", readBytes);
			return 0;
		}
	}
		
	return 0;	
}
