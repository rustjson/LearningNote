package main

import (
	"github.com/davecheney/profile"
	"net/http"
	_ "net/http/pprof"
	_ "time"
)

func test() {
	for i := 0; i < 100; i++ {
		println("test")
	}
}

func main() {
	cfg := profile.Config{
		MemProfile:     true,
		CPUProfile:     true,
		BlockProfile:   true,
		ProfilePath:    ".",
		NoShutdownHook: true,
	}

	defer profile.Start(&cfg).Stop()
	c := make(chan int)
	go func() {
		http.ListenAndServe("0.0.0.0:6060", nil)
	}()
	<-c
	for i := 0; i < 10000; i++ {
		go func(i int) {
			test()
			c <- i
		}(i)
	}
	<-c //for http
	for i := 0; i < 10000; i++ {
		<-c
	}

	//println(http.ListenAndServe("localhost:6062", nil))
}
