
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
## Q
```
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



:tiger: 王小湖北，你给我站住！:kissing_heart: 

好吧别理我，我今天早上没吃:pill: 






