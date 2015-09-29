#difference of proc_close & proc_terminate
```php
<?php
$desc = []; 
$p = proc_open("nc -l 8888", $desc, $pipes);
sleep(10);
echo "Running on line ".__LINE__."\n";
$ret = proc_close($p);
echo "died\n";
exit(0);
```

Run about script, It will stuck at ```proc_close()```
Here is the result of strace:
```bash
nanosleep({10, 0}, 0x7fff332df5d0)      = 0
getrusage(RUSAGE_SELF, {ru_utime={0, 261960}, ru_stime={0, 43993}, ...}) = 0
write(1, "Running on line 5\n", 18Running on line 5
)     = 18
getrusage(RUSAGE_SELF, {ru_utime={0, 261960}, ru_stime={0, 43993}, ...}) = 0
gettimeofday({1443497830, 914209}, NULL) = 0
wait4(12845, 
```
See ?
proc_close do a ```wait4``` syscall. wait for child exit;

Then how about ```proc_terminate()```?
```php
<?php
$desc = []; 
$p = proc_open("nc -l 8888", $desc, $pipes);
sleep(10);
echo "Running on line ".__LINE__."\n";
$ret = proc_terminate($p);
echo "died\n";
exit(0);
```

```bash
#result of strace
nanosleep({10, 0}, 0x7fff2235f7f0)      = 0  
getrusage(RUSAGE_SELF, {ru_utime={0, 260960}, ru_stime={0, 49992}, ...}) = 0
write(1, "Running on line 5\n", 18Running on line 5
)     = 18 
getrusage(RUSAGE_SELF, {ru_utime={0, 260960}, ru_stime={0, 49992}, ...}) = 0
gettimeofday({1443498007, 487133}, NULL) = 0
kill(12882, SIGTERM)                    = 0  
--- SIGCHLD (Child exited) @ 0 (0) ---
getrusage(RUSAGE_SELF, {ru_utime={0, 260960}, ru_stime={0, 49992}, ...}) = 0
write(1, "died\n", 5died
```
```proc_terminate()``` send a signal kill to child process!
