package main

import (
	"fmt"
	"os"
	"io"
	"net/http"
	"net/http/httputil"
//	"io/ioutil"
)


func main() {
	url := os.Args[1]

	req, _:= http.NewRequest("GET", url, nil)

	dump, _ := httputil.DumpRequestOut(req, true)
	fmt.Println("Request:")
	fmt.Printf("%s", dump)

	client := new(http.Client)
	resp, _ := client.Do(req)
	dumpResp, _ := httputil.DumpResponse(resp, true)
	fmt.Println("Response:")
	fmt.Printf("%s", dumpResp)

	file, _ := os.Create("save.jpg")
	io.Copy(file, resp.Body)
}
