#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <sched.h>

static void * ChildFunc(void *arg) {
	sleep(100);
}
int main() {
	
	int pid = -1;
	//clone(SIGCHLD, 0); CLONE_FS
	clone(ChildFunc, CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND, 0);
	sleep(200);
}
