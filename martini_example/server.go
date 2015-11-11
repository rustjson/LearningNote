package main

import "github.com/go-martini/martini"

func main() {
	martini.Env = martini.Prod
	m := martini.Classic()
	m.Get("/", func() string {
		return "Hello World!"
	})
	m.Use(martini.Static("public"))
	m.Run()
}
