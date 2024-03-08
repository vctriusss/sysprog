package utils

import (
	"bufio"
	"io"
)


func ScanVals[T any](file io.Reader, fn func(string) T, n int) []T {
	res := make([]T, 0)

	scanner := bufio.NewScanner(file)
	scanner.Split(bufio.ScanWords)
	for scanner.Scan() {
		s := scanner.Text()
		if s != " " {
			res = append(res, fn(s))
		}
		if len(res) == n {
			break
		}
	}
	return res
}


func WriteVals(file io.Writer, vals[]string) {
	writer := bufio.NewWriter(file)

	for _, val := range vals {
		_, err := writer.WriteString(val + " ")
		if err != nil {
			Err("Error writing to file: %v", err)
		}
	}
	writer.Flush()
}