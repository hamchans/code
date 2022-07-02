package args

import (
	"fmt"
	"os"
	"m/data"
)

func Checkargs(args []string) data.Socket {
    switch len(args) {
    case 1, 2:
        fmt.Println("Few argument!: Please input Server IP address & Server Port\n")
        os.Exit(1)
        return data.Socket{"-1", "-1"}
    case 3:
        fmt.Println("Correct Syntax!\n")
        return data.Socket{args[1], args[2]}
    default:
        fmt.Println("Too argument!: Please input in correct sysntax.\n")
        os.Exit(1)
        return data.Socket{"-1", "-1"}
    }
}
