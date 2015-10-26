
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
---
## How to get source code of basic command in ubuntu like ```ls top``` etc
## 在ubuntu下如何找到基本命令(```ls top```等)的源码
```bash
> dpkg -S /bin/ls
coreutils: /bin/ls
#So coreutils is the package that contains command ls.
```
Then goto this page [Ubuntu – Ubuntu Packages Search](http://packages.ubuntu.com/), search it by using that keyword

然后到这个页面[Ubuntu – Ubuntu Packages Search](http://packages.ubuntu.com/)里用得到的关键字搜索

top's package download page： http://packages.ubuntu.com/trusty/procps ，On the right side!

top所在包的下载页面 http://packages.ubuntu.com/trusty/procps 右边有一个souce下载。是他是他就是他！

---

## What if the difference of Shared and Clean?
```
Shared_Clean:          0 kB
Shared_Dirty:          0 kB
Private_Clean:         0 kB
Private_Dirty:         0 kB
Referenced:            0 kB
Anonymous:             0 kB
AnonHugePages:         0 kB
Swap:                  0 kB
KernelPageSize:        4 kB
MMUPageSize:           4 kB
Locked:                0 kB
```
Both Shared and Clean has two part of Clean and Dirty. But what does that respent?
see [stackexchange](http://unix.stackexchange.com/questions/33381/getting-information-about-a-process-memory-usage-from-proc-pid-smaps)

Dirty 表示页是脏的, 即与磁盘后备文件的内容不一致, 需要(或正在)写回后备设备
Clean 表示页的内容和后备是一致的.

See [fs/proc/task_mmu.c#show_smap](https://github.com/torvalds/linux/blob/v3.18/fs/proc/task_mmu.c#L587)
See [/mm/pagewalk.c#walk_page_range](https://github.com/torvalds/linux/blob/v3.18/mm/pagewalk.c#L167)

For both difference of private and shared, dirty and clean :
See [private_dirty_modify](https://github.com/torvalds/linux/blob/v3.18/fs/proc/task_mmu.c#L496)

When talking about private and shared, that is simple, as code shows, 
```
if (page_mapcount(page)>=2) {
    //shared
} else {
    //private
}
```

---

## :tiger: 王小湖北，你给我站住！:kissing_heart: 

## 好吧别理我，我今天早上没吃:pill: 






