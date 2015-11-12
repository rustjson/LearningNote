package main

import (
	_ "fmt"
	"github.com/davecheney/profile"
	_ "net/http"
	_ "net/http/pprof"
	_ "time"
)

func test() int {
	j := 0
	for i := 0; i < 1000000; i++ {
		j *= i
	}
	return (j / 3)
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
		//http.ListenAndServe("0.0.0.0:6060", nil)
	}()
	//<-c
	for i := 0; i < 1000; i++ {
		go func(i int) {
			//fmt.Println(test())
			test()
			c <- i
		}(i)
	}
	//<-c //for http
	for i := 0; i < 1000; i++ {
		<-c
	}

	//println(http.ListenAndServe("localhost:6062", nil))
}
