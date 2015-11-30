b   __do_page_fault
b  do_coredump

struct mm_struct *mm;

So in userspace:
```
sbrk(0) = mm->brk

//Call sbrk(0) is just get the boundey of mm_struct->brk.
And here, mm = current->mm
```





