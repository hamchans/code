package server

import (
	"net"
	"log"
	"io"
	"time"
)

func LogFatal(err error) {
	if err != nil {
		log.Fatal(err)
	}
}

func ReceiveTCPConnection(listener *net.TCPListener) {
	for {
		conn, err := listener.AcceptTCP()

		LogFatal(err)
		go echoHandler(conn)
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
