package main

import (
	"fmt"
	"io"
	"log"
	"net"
	"os"
	"m/args"
	"m/data"
)

func response(dst io.Writer, src io.Reader) {
	if _, err := io.Copy(dst, src); err != nil {
		log.Fatal(err)
	}
}

var (
	dest data.Socket
)

func main() {
	// whether Number of Command Line variable is correct or not
	dest = args.Checkargs(os.Args) // dest.Port is Server IP address, dest.Ip is Server Port number

	// whether IP address is correct or not, correct is 0 but not is -1
	if args.CheckIP(dest.Ip) == -1 {
		fmt.Println("Please input correct IP address!")
		os.Exit(-1)
	}

	
	destIP := os.Args[1]
	fmt.Printf("%s\n", destIP)

	conn, err := net.Dial("tcp", destIP + ":8080")
	if err != nil {
		log.Fatal(err)
	}

	response(os.Stdout, conn)
}

