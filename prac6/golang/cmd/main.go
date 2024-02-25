package main

import (
	"fmt"
	"math"
	"os"
	. "prac6/pkg/utils"
	"sync"
	"time"
)


func getNWrite(i int) int {
	if i == N-1 {
		return nLast
	}
	return nEach
}

var (
	N     int
	nEach int
	nLast int
	filename string
	totalSum float64
	mu sync.Mutex
)

func main() {
	filename, N = ParseArgs(os.Args, "subprocesses")

	file, err := os.Open(filename)
	CheckFileErr(err, filename)

	nums := ScanVals[string](file, func(s string) string {
		ParseUint(s)
		return s
	}, 0)

	totalNums := len(nums)
	
	if totalNums < 2 {
		Err("There must be 2 or more numbers in a file")
	}
	if N > totalNums/2 {
		fmt.Println("There are too many child processes, decreasing...")
		N = totalNums / 2
	}

	nEach = totalNums / N
	nLast = totalNums - (N-1)*nEach

	var wg sync.WaitGroup

	for i := 0; i < N; i++ {
		nWrite := getNWrite(i)

		wg.Add(1)
		go func(nums []string) {
			defer wg.Done()

			var localSum float64

			for _, nStr := range nums {
				n := ParseUint(nStr)
				localSum += math.Sqrt(float64(n))
			}

			mu.Lock()
			totalSum += localSum
			mu.Unlock()			

			time.Sleep(20 * time.Second)
			
		}(nums[i*nEach:i*nEach+nWrite])
	}

	wg.Wait()

	fmt.Printf("The final sum of square roots is %f\n", totalSum)
	os.Exit(0)
}
