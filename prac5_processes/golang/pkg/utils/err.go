package utils

import (
	"fmt"
	"os"
)


func Err(format string, args ...any) {
	fmt.Fprintf(os.Stderr, format+"\n", args...)
	os.Exit(1)
}


func CheckFileErr(err error, filename string) {
	if err != nil {
		Err("Error opening a file %s: %v", filename, err)
	}
}