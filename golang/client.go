package main

import (
	"fmt"
	"log"
	"net"
	"os"
	"m/args"
	"m/data"
	"bufio"
)

var (
	dest data.Socket
)

var sc = bufio.NewScanner(os.Stdin)

func nextLine() string {
	sc.Scan()
	return sc.Text()
}

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

	str := nextLine()

	_, err = conn.Write([]byte(str))
    if err != nil {
        log.Fatal(err)
    }

	buf := make([]byte, 1024)
	count, err := conn.Read(buf)
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println(string(buf[:count]))
}
