package main

import (
	"fmt"
	"net"
	"os"
	"m/args"
	"m/data"
	"m/error"
	"strings"
	"os/exec"
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
	error.ErrorLog(err)

	listener, err := net.ListenTCP("tcp", tcpAddr)
	error.ErrorLog(err)

	for {
		conn, err := listener.Accept()
		error.ErrorLog(err)

		buf := make([]byte, 1024)
		count, err := conn.Read(buf)
		error.ErrorLog(err)
		fmt.Println(string(buf[:count]))

		str := strings.Split(string(buf[:count]), " ")
		cmd := exec.Command(str[0], str[1:]...)
		stdin, _ := cmd.StdinPipe()
		stdin.Close()
		out, err := cmd.Output()
		fmt.Printf("From %s:%s\n", self.Ip, self.Port)
		if err != nil {
			fmt.Println("Error\n")
			fmt.Printf("%s\n", err)
		} else {
			fmt.Println("Correct\n")
			fmt.Printf("%s", out)
		}

		_, err = conn.Write([]byte(string(out)))
		error.ErrorLog(err)
		conn.Close()
	}
}
