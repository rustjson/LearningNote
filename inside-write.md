without O_APPEND flags

```
(gdb) p file.f_flags 
$5 = 32770

(gdb) p /t file.f_flags
$6 = 1000000000000010

```


With O_APPEND flags

```
(gdb) p file.f_flags 
$9 = 33794

#binary

(gdb) p /t file.f_flags 
$16 = 1000010000000010
```

