package main

import (
	"fmt"
	"os"
	"bufio"
	"os/exec"
	"strings"
)

var sc = bufio.NewScanner(os.Stdin)

func nextLine() string {
	sc.Scan()
	return sc.Text()
}

func main(){
	str := strings.Split(nextLine(), " ")
	cmd := exec.Command(str[0], str[1:]...)
	stdin, _ := cmd.StdinPipe()
	stdin.Close()
	out, err := cmd.Output()
//	cmd.Run()
	if err != nil {
		fmt.Println("Error\n")
		fmt.Printf("%s\n", err)
	} else {
		fmt.Println("Correct\n")
		fmt.Printf("%s", out)
	}
}
