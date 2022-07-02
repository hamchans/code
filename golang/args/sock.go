package args

import (
	"strings"
	"strconv"
	"m/data"
)

func CheckIP(s string) int {
	ip := strings.Split(s, ".")
	for i := 0; i < 4; i ++ {
		num, _ := strconv.Atoi(ip[i])
		if num < data.Min8bit || num > data.Max8bit {
			return -1
		}
	}
	return 0
}

func CheckPort(p string) int {
	num, _ := strconv.Atoi(p)
	if num < data.Minport || num > data.Maxport {
		return -1
	}
	return 0
}
