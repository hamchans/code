package main

import (
	"fmt"
	"net"
	"os"
	"m/server"
	"m/args"
	"m/data"
)

var (
	self data.Socket
)

func main() {
	// whether Number of Command Line variable is correct or not
	self = args.Checkargs(os.Args) // self.Ip is my IP address, self.Port is my Port number

	// whether IP address is correct or not, correct is 0 but not is -1
	if args.CheckIP(self.Ip) == -1 {
		fmt.Println("Please input correct IP address!")
		os.Exit(-1)
	}
	// whether Port number is correct or not, correct is 0 but not is -1
	if args.CheckPort(self.Port) == -1 {
		fmt.Println("Please input correct Port number!")
		os.Exit(-1)
	}

	tcpAddr, err := net.ResolveTCPAddr("tcp", self.Ip + ":" + self.Port)
	server.LogFatal(err)

	listener, err := net.ListenTCP("tcp", tcpAddr)
	server.LogFatal(err)

	fmt.Println("Start TCP Server...")
	server.ReceiveTCPConnection(listener)
}
