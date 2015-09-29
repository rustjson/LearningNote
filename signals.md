#How can I check what signals a process is listening to?

Under Linux, you can find the PID of your process, then look at /proc/$PID/status. It contains lines describing which signals are blocked (SigBlk), ignored (SigIgn), or caught (SigCgt).
```
# cat /proc/1/status
...
SigBlk: 0000000000000000
SigIgn: fffffffe57f0d8fc
SigCgt: 00000000280b2603
...
The number to the right is a bitmask. If you convert it from hex to binary, each 1-bit represents a caught signal, counting from right to left starting with 1. So by interpreting the SigCgt line, we can see that my init process is catching the following signals:

00000000280b2603 ==> 101000000010110010011000000011
                     | |       | ||  |  ||       |`->  1 = SIGHUP
                     | |       | ||  |  ||       `-->  2 = SIGINT
                     | |       | ||  |  |`----------> 10 = SIGUSR1
                     | |       | ||  |  `-----------> 11 = SIGSEGV
                     | |       | ||  `--------------> 14 = SIGALRM
                     | |       | |`-----------------> 17 = SIGCHLD
                     | |       | `------------------> 18 = SIGCONT
                     | |       `--------------------> 20 = SIGTSTP
                     | `----------------------------> 28 = SIGWINCH
                     `------------------------------> 30 = SIGPWR
(I found the number-to-name mapping by running kill -l from bash.)
```
[link](http://unix.stackexchange.com/questions/85364/how-can-i-check-what-signals-a-process-is-listening-to)
