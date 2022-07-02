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

func main() {
	tcpAddr, err := net.ResolveTCPAddr("tcp", "192.168.11.23:8080")
	logFatal(err)

	listener, err := net.ListenTCP("tcp", tcpAddr)
	logFatal(err)

	fmt.Println("Start TCP Server...")
	receiveTCPConnection(listener)
}
