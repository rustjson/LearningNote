### How ListenAndServe works in go

<p>I cares about performance a lots, most of the programming language provide us some profiling tools to play with.
Like PHP's xhprof, I think "C'est super!"(means it's great!) </p>
<p>[This post](https://medium.com/@tjholowaychuk/profiling-golang-851db2d9ae24) explains very well about how to profiling golang, But today, I would like to take a inside look about that.</p>

<p>Let's take a look at the source code of ```net/http/pprof/pprof.go```, in the [```init()``` function](https://github.com/golang/go/blob/release-branch.go1.5/src/net/http/pprof/pprof.go#L68-L72):

```
    http.Handle("/debug/pprof/", http.HandlerFunc(Index))
    http.Handle("/debug/pprof/cmdline", http.HandlerFunc(Cmdline))
    http.Handle("/debug/pprof/profile", http.HandlerFunc(Profile))
    http.Handle("/debug/pprof/symbol", http.HandlerFunc(Symbol))
    http.Handle("/debug/pprof/trace", http.HandlerFunc(Trace))
```
At first I though here shoud have a a handler to handle ```debug/pprof/goroutine```, and thought it was being removed since some version, But I was wrong,
It was actually go into ```Index``` handler.
So a use gdb to restart it and add a break point at funciont ```Index```, after serverl execution,  my backtrace like this:


```
#0  net/http/pprof.handler.ServeHTTP (name=..., w=..., r=0xc820154700) at /usr/local/go/src/net/http/pprof/pprof.go:195
#1  0x00000000004983d0 in net/http/pprof.Index (w=..., r=0xc820154700) at /usr/local/go/src/net/http/pprof/pprof.go:211
#2  0x00000000004765ca in net/http.HandlerFunc.ServeHTTP (f={void (net/http.ResponseWriter, struct net/http.Request *)} 0xc820171b78, w=..., 
    r=0xc820154700) at /usr/local/go/src/net/http/server.go:1422
#3  0x0000000000477e2d in net/http.(*ServeMux).ServeHTTP (mux=0xc8200748d0, w=..., r=0xc820154700)
    at /usr/local/go/src/net/http/server.go:1699
#4  0x000000000047889e in net/http.serverHandler.ServeHTTP (sh=..., rw=..., req=0xc820154700) at /usr/local/go/src/net/http/server.go:1862
#5  0x000000000047608e in net/http.(*conn).serve (c=0xc8200e49a0) at /usr/local/go/src/net/http/server.go:1361
#6  0x000000000045e581 in runtime.goexit () at /usr/local/go/src/runtime/asm_amd64.s:1696

```

So, It was ```pprof.Lookup("goroutine")``` returns all the goroutine.
In [this function)(https://github.com/golang/go/blob/release-branch.go1.5/src/runtime/pprof/pprof.go#L133):

```
lockProfiles()
defer unlockProfiles()
return profiles.m[name]
```
It returns profiles.m["goroutine"], and this is easy to find [here](https://github.com/golang/go/blob/release-branch.go1.5/src/runtime/pprof/pprof.go#L70-L72):
```                                                                                                                                                                                                          
var goroutineProfile = &Profile{
    name:  "goroutine",
    count: countGoroutine,
    write: writeGoroutine,
}
```
write points to the function. that call by [p.WriteTo](https://github.com/golang/go/blob/release-branch.go1.5/src/net/http/pprof/pprof.go#L199):
It's not difficult, but sometime confusing.
It will finally execue ```runtime.GoroutineProfile``` locate in [here](https://github.com/golang/go/blob/release-branch.go1.5/src/runtime/mprof.go#L519-L549)


---
### How net.http.pprof works.
It has 5 handlers:
[pprof.init()](https://github.com/golang/go/blob/release-branch.go1.5/src/net/http/pprof/pprof.go#L68-L72)

```
	http.Handle("/debug/pprof/", http.HandlerFunc(Index))
	http.Handle("/debug/pprof/cmdline", http.HandlerFunc(Cmdline))
	http.Handle("/debug/pprof/profile", http.HandlerFunc(Profile))
	http.Handle("/debug/pprof/symbol", http.HandlerFunc(Symbol))
	http.Handle("/debug/pprof/trace", http.HandlerFunc(Trace))
```

I wonder why ```curl localhost:6060/goroutine``` could print all the goroutine. cause there is no handler of ```goroutine```. So, break at handler of Index:
We will get this:

```
#0  net/http/pprof.handler.ServeHTTP (name=..., w=..., r=0xc820154700) at /usr/local/go/src/net/http/pprof/pprof.go:195
#1  0x00000000004983d0 in net/http/pprof.Index (w=..., r=0xc820154700) at /usr/local/go/src/net/http/pprof/pprof.go:211
#2  0x00000000004765ca in net/http.HandlerFunc.ServeHTTP (f={void (net/http.ResponseWriter, struct net/http.Request *)} 0xc820171b78, w=..., 
    r=0xc820154700) at /usr/local/go/src/net/http/server.go:1422
#3  0x0000000000477e2d in net/http.(*ServeMux).ServeHTTP (mux=0xc8200748d0, w=..., r=0xc820154700)
    at /usr/local/go/src/net/http/server.go:1699
#4  0x000000000047889e in net/http.serverHandler.ServeHTTP (sh=..., rw=..., req=0xc820154700) at /usr/local/go/src/net/http/server.go:1862
#5  0x000000000047608e in net/http.(*conn).serve (c=0xc8200e49a0) at /usr/local/go/src/net/http/server.go:1361
#6  0x000000000045e581 in runtime.goexit () at /usr/local/go/src/runtime/asm_amd64.s:1696

```
Now it is easy to figure out how it works

## Inside go Sche:
In file [proc1.go#sysmon](https://github.com/golang/go/blob/release-branch.go1.5/src/runtime/proc1.go#L2959)

this function call ```schedtrace(debug.scheddetail > 0)``` which produce some output like:

```
SCHED 522ms: gomaxprocs=1 idleprocs=0 threads=5 spinningthreads=0 idlethreads=0 runqueue=512 [123]
SCHED 532ms: gomaxprocs=1 idleprocs=0 threads=5 spinningthreads=0 idlethreads=0 runqueue=512 [116]
SCHED 542ms: gomaxprocs=1 idleprocs=0 threads=5 spinningthreads=0 idlethreads=0 runqueue=512 [109]
SCHED 552ms: gomaxprocs=1 idleprocs=0 threads=5 spinningthreads=0 idlethreads=0 runqueue=512 [101]
SCHED 562ms: gomaxprocs=1 idleprocs=0 threads=5 spinningthreads=0 idlethreads=0 runqueue=512 [94]
```

So I think sysmon() was being called very often. Let's figure out who is his caller.

```
./runtime/proc.go-	systemstack(func() {
./runtime/proc.go:		newm(sysmon, nil)
```

Looks like ```systemstack()``` is a very foundermental function ?

