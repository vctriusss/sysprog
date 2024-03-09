package main

import (
	"fmt"
	"math"
	"os"
	. "prac7/pkg/utils"
	"strconv"
	"sync"

	// "time"
	"github.com/ghetzel/shmtool/shm"
)

func main() {
	N, _ := strconv.Atoi(os.Args[1])
	switch os.Args[2] {
	case "pipes":
		results := ScanVals(os.Stdin, func(s string) float64 {
			n := ParseUint(s)
			return math.Sqrt(float64(n))
		}, N)

		var sum float64
		for _, n := range results {
			sum += n
		}
		// time.Sleep(10 * time.Second)

		WriteVals(os.Stdout, []string{strconv.FormatFloat(sum, 'f', 32, 64)})

	case "shm":
		numsMemId, _ := strconv.Atoi(os.Args[3])
		resMemId, _ := strconv.Atoi(os.Args[4])
		offsetNums, _ := strconv.Atoi(os.Args[5])

		// time.Sleep(10 * time.Second)

		nums_mem, err := shm.Open(numsMemId)
		if err != nil {
			Err("Failed to open shared memory")
		}

		res_mem, err := shm.Open(resMemId)
		if err != nil {
			Err("Failed to open shared memory")
		}
		var sum float64

		var mu sync.Mutex

		mu.Lock()
		sum = ScanVals(res_mem, ParseFloat, 1)[0]

		results := ScanVals(nums_mem, func(s string) float64 {
			n := ParseUint(s)
			return math.Sqrt(float64(n))
		}, -1)[offsetNums : offsetNums+N]

		for _, n := range results {
			sum += n
		}
		res_mem.Reset()

		WriteVals(res_mem, []string{fmt.Sprintf("%10.10f", sum)})
		mu.Unlock()
	}

	os.Exit(0)
}
