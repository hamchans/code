package main

import (
	"fmt"
	"io"
	"log"
	"net"
	"os"
)

func response(dst io.Writer, src io.Reader) {
	if _, err := io.Copy(dst, src); err != nil {
		log.Fatal(err)
	}
}

func main() {
	fmt.Println("count:", len(os.Args))

	for i, v := range os.Args {
		fmt.Printf("args[%d] -> %s\n", i, v)
	}
	destIP := os.Args[1]
	fmt.Printf("%s\n", destIP)

	conn, err := net.Dial("tcp", destIP + ":8080")
	if err != nil {
		log.Fatal(err)
	}

	response(os.Stdout, conn)
}

