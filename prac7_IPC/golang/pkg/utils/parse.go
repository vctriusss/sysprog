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


func ParseArgs(args []string) (string, int, string) {
	var (
		filename string
		N int
		mode string
		err error
	)
	if len(os.Args) != 4 {
		Err("Please specify file to read, number of processes and mode accordingly (e.g ./bin_file <filename> <N> <mode>)")
	} else {
		filename = os.Args[1]
		N, err = strconv.Atoi(os.Args[2])
		if err != nil || N <= 0{
			Err("Number of processes must be a positive integer")
		}
		mode = os.Args[3]

	}
	return filename, N, mode
}