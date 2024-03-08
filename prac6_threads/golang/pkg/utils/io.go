package utils

import (
	"os"
	"bufio"
)


func ScanVals[T any](file *os.File, fn func(string) T, skip int) []T {
	res := make([]T, 0)

	scanner := bufio.NewScanner(file)
	scanner.Split(bufio.ScanWords)
	for i := 0; i < skip; i++ {
		scanner.Scan()
	}
	for scanner.Scan() {
		s := scanner.Text()
		if s != " " {
			res = append(res, fn(s))
		}
	}
	file.Close()
	return res
}


func WriteVals(file *os.File, vals[]string, newline bool) {
	writer := bufio.NewWriter(file)
	if newline {
		writer.WriteString("\n")
	}
	last := len(vals) - 1
	for i, val := range vals {
		var num string
		if i == last {
			num = val
		} else {
			num = val + " "
		}

		_, err := writer.WriteString(num)
		if err != nil {
			Err("Error writing to file: %v", err)
		}
	}
	writer.Flush()
	file.Close()
}