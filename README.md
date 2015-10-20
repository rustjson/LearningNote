
# 交个朋友 [点这里](https://github.com/JasonSunan/frendsMaking/issues)

#lsof usage
Show port listen
```bash
#show port 80
lsof -i :80

COMMAND   PID   USER   FD   TYPE DEVICE SIZE/OFF NODE NAME
nginx   14052   root    6u  IPv4 303704      0t0  TCP *:http (LISTEN)
nginx   14053 nobody    6u  IPv4 303704      0t0  TCP *:http (LISTEN)


```

# example of update-alternatives 
```bash
#add 
sudo update-alternatives --install /usr/bin/phpize phpize /home/jason/php/5.4.11/bin/phpize 1 
#config
sudo update-alternatives --config phpize

#remove
sudo update-alternatives --remove phpize /usr/local/php/5.4.11/bin/php-phpize
```

# source of brk
https://github.com/torvalds/linux/blob/master/mm/mmap.c#L286
## Q: Why this won't seg me?
## Q：为什么操作(base + 1)不会出现段错误？
```c
#include <stdio.h>
#include <unistd.h>

int main() {
    int *base = sbrk(0);
        
    brk(base + 1);//expand 4bytes(sizeof int) memory 
                                                                        
    printf("base.addr=%p\n", base);
    printf("base+1.addr=%p\n", base + 1); 
    printf("After, brk(), current.addr=%p\n", sbrk(0));
    
    *base = 1; #comment out brk, It will seg here.
    *(base + 1)  = 1;#Why It won't seg me here.

    return 0;
}
```

## A: Because it is page aligned!
## A: 因为brk内部也有页对齐！

```C
#include <stdio.h>
#include <unistd.h>

int main() {
    int *base = sbrk(0);
    int pagesize = getpagesize();

    brk(base + 1);//Here are page aligned!!!!!!!!                       

    printf("pagesize=%d\n", pagesize);
    printf("base.addr=%p\n", base);
    printf("base+1.addr=%p\n", base + 1); 
    printf("After, brk(), current.addr=%p\n", sbrk(0));
    
    *base = 1;
    *(base + pagesize/sizeof(int) - 1)   = 1;
    printf("Next, please seg me!\n");
    *(base + pagesize/sizeof(int))   = 1;
    //sleep(10000); sleep a long time, I need to get
    //the virtual address range of the heap of this program
    //sudo cat /proc/PID/maps |grep heap
    return 0;
}

```
## A:Another proof:
## A：另外一个证明的方法：

```bash
sudo cat /proc/PID/maps |grep heap
#This command could get the virtual address of heap, result are the same
#with output of above progrma 
#通过这个命令拿到堆的地址，和上面程序打印出来的地址完全一模一样
```


:tiger: 王小湖北，你给我站住！:kissing_heart: 

好吧别理我，我今天早上没吃:pill: 






