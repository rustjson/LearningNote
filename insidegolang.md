# Inside golang 

## How golang being build
First, build a bootstrap tool called dist:
https://github.com/golang/go/blob/release-branch.go1.5/src/make.bash#L121
Be aware of $GOROOT_BOOTSTRAP, In my currently env i build through:
```GOROOT_BOOTSTRAP=/home/sunan/go/src/go1.4/ ./make.bash```

That's where my go1.4 placed

try to add ```exit``` at [line 122](https://github.com/golang/go/blob/release-branch.go1.5/src/make.bash#L122)
And add ```exit``` @[line 146](https://github.com/golang/go/blob/release-branch.go1.5/src/make.bash#L146)

Then, We need to execute is dist by ourself:

```GOROOT_BOOTSTRAP=/home/sunan/go/src/go1.4/ ./cmd/dist/dist bootstrap -a -v```

It will Building Go toolchain using /home/sunan/go/src/go1.4 for us



