
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

ref: [How much memory are applications really using?](http://www.eqware.net/Articles/CapturingProcessMemoryUsageUnderLinux/)
[pagemap - The Linux Kernel Archives](https://www.kernel.org/doc/Documentation/vm/pagemap.txt)



```fcntl(fd, F_SETFD, FD_CLOEXEC)```

It sets the close-on-exec flag for the file descriptor, which causes the file descriptor to be automatically (and atomically) closed when any of the exec-family functions succeed.[stackoverflow](http://stackoverflow.com/questions/6125068/what-does-the-fd-cloexec-fcntl-flag-do)

## What happend to fd when a parent process exit without ```waitpid()``` his child.

```C
#include <stdio.h>
#include <unistd.h>

int main() {
    int pid = -1; 
    char buf[10] = {0};

    pid = fork();
    if (pid == -1) {
        perror("fork");
    }   
    if (pid == 0) {//child
        printf("[child]sleeping\n");
        sleep(2);//sleep to wait parent exit;
        write(1, "[child]nihao\n", 13);
        if (read(0, buf, 1) == -1) {
            printf("[child]ops, read error\n");
        }   
        _exit(0);
    } else {
        printf("[parent]exit\n");
        //if (waitpid(pid, NULL, 0) < 0) {
        //     perror("waitpid error\n");
        //} 
        _exit(0);   
    }   
}
```

```bash
sunan at Sunan-ubuntu in ~/c/more
○ ./fd-on-close 
[parent]exit
[child]sleeping

sunan at Sunan-ubuntu in ~/c/more
○ [child]nihao
[child]ops, read error

```
See ? You can still write, but can't read.

---
### Why async curl slower then curl on a 8 core machine when curl a static page servie by nginx

---
### Why execute a new command by using ```./a.out``` always create a new process group ?
### 为什么```./```执行出来的命令总是一个新的进程组？

### Guess:
### 猜：
Inside ./a.out, bash create execute ```clone()``` and then ```setpgid(0, PID)``` then ```execve(a.out...)```

当你执行./a.out的时候，系统一次执行```clone()```然后，```setpgid(0, PID)``` 然后 ```execve(a.out...)```

### Prof:
### 证明：
```C
//newgroup.c
#include <stdlib.h>
#include <stdio.h>

int main() {
	printf("This is a marker\n");
	sleep(1000);
	return 0;
}
```
Let's compile this file to newgroup and then use ```strace``` to trace our zsh,

In my test, the PID of zsh is 9421, so simply open a new terminal and execute this:

```bash
sudo strace -esetpgid,execve,write,clone -p 9421 -f -v
```
And then execute our newgroup:

```bash
./newgroup
```

Outputs looks like this:
```bash
Process 9421 attached



write(1, "\33[?1l\33>", 7)              = 7
write(10, "\r\n", 2)                    = 2
write(3, ": 1446459559:0;./newgroup \n", 27) = 27
write(1, "\33]2;./newgroup\7", 15)      = 15
write(1, "\33]1;./newgroup\7", 15)      = 15
clone(Process 17943 attached
child_stack=0, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLD, child_tidptr=0x7f4de73aba10) = 17943
[pid 17943] setpgid(0, 17943)           = 0
[pid 17943] execve("./newgroup", ["./newgroup"], ["SESSION_MANAGER=local/Sunan-ubun"..., "XDG_SESSION_ID=c2", "LC_IDENTIFICATION=en_US.UTF-8", "XDG_SEAT=seat0", "DISPLAY=:0", "JOB=dbus", "COLORTERM=gnome-terminal", "GNOME_KEYRING_CONTROL=/run/user/"..., "GNOME_DESKTOP_SESSION_ID=this-is"..., "DEFAULTS_PATH=/usr/share/gconf/u"..., "QT_QPA_PLATFORMTHEME=appmenu-qt5", "LOGNAME=sunan", "TEXTDOMAIN=im-config", "INSTANCE=", "LC_TIME=en_US.UTF-8", "SHELL=/bin/zsh", "PATH=/home/sunan/google-cloud-sd"..., "LC_NUMERIC=en_US.UTF-8", "LC_PAPER=en_US.UTF-8", "IM_CONFIG_PHASE=1", "TEXTDOMAINDIR=/usr/share/locale/", "CLUTTER_IM_MODULE=xim", "QT4_IM_MODULE=fcitx", "XDG_SESSION_PATH=/org/freedeskto"..., "SESSION=ubuntu", "SSH_AUTH_SOCK=/run/user/1000/key"..., "XDG_MENU_PREFIX=gnome-", "XAUTHORITY=/home/sunan/.Xauthori"..., "QT_IM_MODULE=xim", "GDMSESSION=ubuntu", "XMODIFIERS=@im=fcitx", "LC_MEASUREMENT=en_US.UTF-8", "LC_ADDRESS=en_US.UTF-8", "XDG_CONFIG_DIRS=/etc/xdg/xdg-ubu"..., "MANDATORY_PATH=/usr/share/gconf/"..., "UPSTART_SESSION=unix:abstract=/c"..., "XDG_RUNTIME_DIR=/run/user/1000", "DESKTOP_SESSION=ubuntu", "GTK_IM_MODULE=fcitx", "GTK_MODULES=overlay-scrollbar:un"..., "USER=sunan", "PWD=/home/sunan/c/more", "VTE_VERSION=3409", "LC_MONETARY=en_US.UTF-8", "HOME=/home/sunan", "XDG_SEAT_PATH=/org/freedesktop/D"..., "XDG_DATA_DIRS=/usr/share/ubuntu:"..., "LANGUAGE=fr_FR:en", "SELINUX_INIT=YES", "COMPIZ_CONFIG_PROFILE=ubuntu", "XDG_GREETER_DATA_DIR=/var/lib/li"..., "COMPIZ_BIN_PATH=/usr/bin/", "LC_NAME=en_US.UTF-8", "LANG=en_US.UTF-8", "GPG_AGENT_INFO=/run/user/1000/ke"..., "SHLVL=1", "WINDOWID=75497483", "XDG_VTNR=7", "GDM_LANG=fr_FR", "SESSIONTYPE=gnome-session", "DBUS_SESSION_BUS_ADDRESS=unix:ab"..., "XDG_CURRENT_DESKTOP=Unity", "TERM=xterm", "GNOME_KEYRING_PID=2567", "LC_TELEPHONE=en_US.UTF-8", "OLDPWD=/home/sunan/c", "ZSH=/home/sunan/.oh-my-zsh", "GOPATH=/home/sunan/golang", "GOROOT=/home/sunan/go/src/go1.5", "PAGER=less", "LESS=-R", "LC_CTYPE=fr_FR.UTF-8", "LSCOLORS=Gxfxcxdxbxegedabagacad", "_=/home/sunan/c/more/./newgroup"]) = 0
[pid 17943] write(1, "This is a marker\n", 17) = 17

```

---

## :tiger: 王小湖北，你给我站住！:kissing_heart: 

## 好吧别理我，我今天早上没吃:pill: 






