package main

import (
	"math"
	"os"
	. "prac5/pkg/utils"
	"strconv"
)



func main() {
	filename, _ := ParseArgs(os.Args, "numbers")

	file, err := os.Open(filename)
	CheckFileErr(err, filename)

	results := ScanVals[float64](file, func(s string) float64 {
		n := ParseUint(s)
		return math.Sqrt(float64(n))
	}, 0)

	var sum float64
	for _, n := range results {
		sum += n
	}

	file, err = os.OpenFile(filename, os.O_APPEND|os.O_WRONLY, 0600)
	CheckFileErr(err, filename)

	defer file.Close()

	WriteVals(file, []string{strconv.FormatFloat(sum, 'f', 32, 64)}, true)

	os.Exit(0)
}
