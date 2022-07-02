package args

import (
	"strings"
	"strconv"
)

func CheckIP(s string) int {
	ip := strings.Split(s, ".")
	for i := 0; i < 4; i ++ {
		num, _ := strconv.Atoi(ip[i])
		if num < 0 || num > 255 {
			return -1
		}
	}
	return 0
}
