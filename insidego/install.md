make.bash 里执行
 ./cmd/dist/dist bootstrap -a -v

这里就是执行/cmd/dist/build.go里的cmdboostrap() 方法.
这个方法里会去执行: cmd/dist/buildtools.go 里的 bootstrapBuildTools()
Building Go toolchain/ 
         Generate const like version = go1.5.1, defaultGOROOT= /home/golang etc
         // Use $GOROOT/pkg/bootstrap as the bootstrap workspace root.
         // We use a subdirectory of $GOROOT/pkg because that's the
         // space within $GOROOT where we store all generated objects

copy 一下文件夹的文件都复制一份到 $GOROOT/pkg/bootstrap/src/, 不过会过一遍bootstrapFixImports()函数.

```
 "asm",
"asm/internal/arch",
"asm/internal/asm",
"asm/internal/flags",
"asm/internal/lex",
"compile",
"compile/internal/amd64",
"compile/internal/arm",
"compile/internal/arm64",
"compile/internal/big",
"compile/internal/gc",
"compile/internal/ppc64",
"compile/internal/x86",
"internal/gcprog",
"internal/obj",
"internal/obj/arm",
"internal/obj/arm64",
"internal/obj/ppc64",
"internal/obj/x86",
"link",
"link/internal/amd64",
"link/internal/arm",
"link/internal/arm64",
"link/internal/ld",
"link/internal/ppc64",
"link/internal/x86",
```


然后执行```GOROOT=//home/sunan/go/src/go1.4/ GOPATH=/home/sunan/github.com/RustJason/LearningNote/go/pkg/bootstrap /home/sunan/go/src/go1.4/bin/go install -v bootstrap/...```
其实就是go install 编译了bootstrap这个package.然后

