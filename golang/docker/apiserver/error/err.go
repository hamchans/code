package error

import (
	"log"
)

func ErrorLog(err error) {
	if err != nil {
		log.Fatal(err)
	}
}
