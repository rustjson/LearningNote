package main

import (
	"fmt"
)

func main() {
	fmt.Println("Hello")
	systemstack(func() {
		print("entersyscall inconsistent ")
		//	throw("entersyscall")
	})
}
