#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define DST "./test.txt"
#define TNUM 20
#define BUF_SIZE 4096
#define COUNT 10
