package utils

import (
	"os"
	"strconv"
)


func ParseUint(s string) uint64 {
	n, err := strconv.ParseUint(s, 10, 64)
	if err != nil {
		Err("Error converting to positive int: %v", err)
	}
	return n
}


func ParseFloat(s string) float64 {
	n, err := strconv.ParseFloat(s, 64)
	if err != nil {
		Err("Error converting to float: %v", err)
	}
	return n
}


func ParseArgs(args []string, what string) (string, int) {
	var (
		filename string
		N int
		err error
	)
	if len(os.Args) != 3 {
		Err("Please specify file to read and number of %s accordingly (e.g ./bin_file <filename> <N>)", what)
	} else {
		filename = os.Args[1]
		N, err = strconv.Atoi(os.Args[2])
		if err != nil || N <= 0{
			Err("Number of %s must be a positive integer", what)
		}
	}
	return filename, N
}