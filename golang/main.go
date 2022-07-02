package main

import (
    "fmt"

    "m/hello"  // ← ここ重要
)

func main() {
    fmt.Println(hello.SayHello()) // パッケージ名 + 最初が大文字の関数
    fmt.Println(hello.Greet())
}
