#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <fcntl.h>



static int epollfd = -1;
static struct epoll_event *epollfds = NULL;
static int sp[2];

static void sig_handler(int signo) /* {{{ */ {

    //saved_errno = errno;
    //s = sig_chars[signo];
    //write(sp[1], &s, sizeof(s));
    //errno = saved_errno;
	printf("in sig_handler\n");
}
/* }}} */

static void sig_soft_quit(int signo) /* {{{ */ {

}
/* }}} */

int main(int argc, char *argv[]) {
	
	
    struct sigaction act;

    if (0 > socketpair(AF_UNIX, SOCK_STREAM, 0, sp)) {
		perror("socketpair");
        return -1; 
    }   

   // if (0 > fd_set_blocked(sp[0], 0) || 0 > fd_set_blocked(sp[1], 0)) {
   // 	perror("fd_set_blok");
   //     return -1; 
   // }   

    if (0 > fcntl(sp[0], F_SETFD, FD_CLOEXEC) || 0 > fcntl(sp[1], F_SETFD, FD_CLOEXEC)) {
		perror("fcntl");
        return -1; 
    }   

    memset(&act, 0, sizeof(act));
    act.sa_handler = sig_handler;
    sigfillset(&act.sa_mask);

    if (0 > sigaction(SIGTERM,  &act, 0) ||
        0 > sigaction(SIGINT,   &act, 0) ||
        0 > sigaction(SIGUSR1,  &act, 0) ||
        0 > sigaction(SIGUSR2,  &act, 0) ||
        0 > sigaction(SIGCHLD,  &act, 0) ||
        0 > sigaction(SIGQUIT,  &act, 0)) {

        return -1; 
    }   



	int pid;
	pid = fork();
	if (pid < 0) {
		perror("Fork");
	}
	if (pid == 0) {
		sleep(20);
		exit(1);	
	} else {
		/* init epoll */
		int max = 1;
		int ret;
		epollfd = epoll_create(max + 1); 
		if (epollfd < 0) {
			perror("epoll_create");
			return -1; 
		}   

		/* allocate fds */
		epollfds = malloc(sizeof(struct epoll_event) * max);
		if (!epollfds) {
			perror("malloc epolfds");
			return -1; 
		}   
		memset(epollfds, 0, sizeof(struct epoll_event) * max);

		/* wait for inconming event or timeout */
		ret = epoll_wait(epollfd, epollfds, max, 33000);	
		if (ret < 0) {
			perror("epoll_wait");
		}
		if (ret > 0) {
			
		}
	}
	
}
