package main

import (
	"fmt"
	"io"
	"log"
	"net"
	"time"
)

func logFatal(err error) {
	if err != nil {
		log.Fatal(err)
	}
}

func receiveTCPConnection(listener *net.TCPListener) {
	for {
		conn, err := listener.AcceptTCP()
		logFatal(err)

		echoHandler(conn)
	}
}

func echoHandler(conn *net.TCPConn) {
	defer conn.Close()
	for {
		_, err := io.WriteString(conn, "response from server\n")
		if err != nil {
			return
		}
		time.Sleep(time.Second)
	}
}

var (
	self data.Socket
)

func main() {
	// whether Number of Command Line variable is correct or not
	self = args.Checkargs(os.Args) // self.Ip is my IP address, self.Port is my Port number

	// whether IP address is correct or not, correct is 0 but not is -1
	if args.checkIP(self.Ip) == -1 {
		fmt.Println("Please input correct IP address!")
		os.Exit(-1))
	}
	// whether Port number is correct or not, correct is 0 but not is -1
	if args.CheckPort(self.Port) == -1 {
		fmt.Println("Please input correct Port number!")
		os.Exit(-1)
	}
	
	tcpAddr, err := net.ResolveTCPAddr("tcp", self.Ip + ":" + self.Port)
	logFatal(err)

	listener, err := net.ListenTCP("tcp", tcpAddr)
	logFatal(err)

	fmt.Println("Start TCP Server...")
	receiveTCPConnection(listener)
}
