### How ListenAndServe works in go

gdb and b: [server.conn.serve](https://github.com/golang/go/blob/release-branch.go1.5/src/net/http/server.go#L1361)

+ stack #3:

```
#0  net/http.HandlerFunc.ServeHTTP (f={void (net/http.ResponseWriter, struct net/http.Request *)} 0xc820171b78, w=..., r=0xc8201542a0)
    at /usr/local/go/src/net/http/server.go:1423
#1  0x0000000000477e2d in net/http.(*ServeMux).ServeHTTP (mux=0xc8200748d0, w=..., r=0xc8201542a0)
    at /usr/local/go/src/net/http/server.go:1699
#2  0x000000000047889e in net/http.serverHandler.ServeHTTP (sh=..., rw=..., req=0xc8201542a0) at /usr/local/go/src/net/http/server.go:1862
#3  0x000000000047608e in net/http.(*conn).serve (c=0xc8200e49a0) at /usr/local/go/src/net/http/server.go:1361
#4  0x000000000045e581 in runtime.goexit () at /usr/local/go/src/runtime/asm_amd64.s:1696

```

That's the handle where being called.

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

