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
	dest = args.Checkargs(os.Args) // dest.Ip is Server IP address, dest.Port is Server Port number

	// whether IP address is correct or not, correct is 0 but not is -1
	if args.CheckIP(dest.Ip) == -1 {
		fmt.Println("Please input correct IP address!")
		os.Exit(-1)
	}
	// whether Port number is correct or not, correct is 0 but not is -1
	if args.CheckPort(dest.Port) == -1 {
		fmt.Println("Please input correct Port number!")
		os.Exit(-1)
	}

	conn, err := net.Dial("tcp", dest.Ip + ":" + dest.Port)
	if err != nil {
		log.Fatal(err)
	}

	response(os.Stdout, conn)
}

