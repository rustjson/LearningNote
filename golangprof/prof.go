package main

import (
	"github.com/davecheney/profile"
	_ "net/http"
	_ "net/http/pprof"
	_ "time"
)

func test() {
	for i := 0; i < 10000; i++ {
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
	go func() {
		//http.ListenAndServe("0.0.0.0:6060", nil)
	}()
	c := make(chan int)
	for i := 0; i < 100; i++ {
		go func(i int) {
			test()
			c <- i
		}(i)
	}
	for i := 0; i < 100; i++ {
		<-c
	}

	//println(http.ListenAndServe("localhost:6062", nil))
}
