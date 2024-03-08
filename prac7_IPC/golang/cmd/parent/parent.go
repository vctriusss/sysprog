package main

import (
	"fmt"
	"io"
	"os"
	"os/exec"
	. "prac7/pkg/utils"
	"strconv"
	"sync"

	"github.com/ghetzel/shmtool/shm"
)

func getNWrite(i int) int {
	if i == N-1 {
		return nLast
	}
	return nEach
}

var (
	N        int
	nEach    int
	nLast    int
	filename string
	mode     int
	modestr  string
)

const (
	ModePipes = iota
	ModeSHM
	IntSize   = 32 / 8
	FloatSize = 64 / 8
)

func main() {
	filename, N, modestr = ParseArgs(os.Args)

	switch modestr {
	case "pipes":
		mode = ModePipes
	case "shm":
		mode = ModeSHM
	default:
		Err("Mode must be `pipes` or `shm`")
	}

	file, err := os.Open(filename)
	CheckFileErr(err, filename)

	nums := ScanVals(file, func(s string) string {
		ParseUint(s)
		return s
	}, -1)

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

	children := make([]*exec.Cmd, N)
	childOutPipes := make([]io.ReadCloser, N)

	var res_mem, nums_mem *shm.Segment

	if mode == ModeSHM {
		nums_mem, err = shm.Create(totalNums * IntSize)
		if err != nil {
			Err("Failed to create shared memory")
		}
		defer nums_mem.Destroy()

		res_mem, err = shm.Create(22)
		if err != nil {
			Err("Failed to create shared memory")
		}
		defer res_mem.Destroy()
		WriteVals(res_mem, []string{fmt.Sprintf("%10.10f", 0.0)})

		WriteVals(nums_mem, nums)
	}

	for i := 0; i < N; i++ {
		nWrite := getNWrite(i)

		switch mode {
		case ModePipes:
			child := exec.Command("./child", strconv.Itoa(nWrite), "pipes")
			childReadPipe, _ := child.StdinPipe()
			childWritePipe, _ := child.StdoutPipe()

			if err := child.Start(); err != nil {
				Err("Failed to start child process")
			}

			childOutPipes[i] = childWritePipe
			children[i] = child

			WriteVals(childReadPipe, nums[i*nEach:i*nEach+nWrite])
		case ModeSHM:
			child := exec.Command("./child", strconv.Itoa(nWrite), "shm", strconv.Itoa(nums_mem.Id), strconv.Itoa(res_mem.Id), strconv.Itoa(i*nEach), strconv.Itoa(i))
			child.Stdout = os.Stdout
			if err := child.Start(); err != nil {
				Err("Failed to start child process")
			}

			children[i] = child

		}
	}
	sqrts := make([]float64, N)

	if mode == ModePipes {
		var wg sync.WaitGroup
		for i := 0; i < N; i++ {
			wg.Add(1)
			go func(i int) {
				defer wg.Done()
				defer childOutPipes[i].Close()

				sqrts[i] = ScanVals(childOutPipes[i], ParseFloat, 1)[0]
			}(i)
		}
		wg.Wait()
	}

	for i, child := range children {
		if err := child.Wait(); err != nil {
			Err("Child process %d finished unsuccessfully: %v", i+1, err)
		}
	}

	if mode == ModeSHM {
		res_mem.Reset()
		sqrts = ScanVals(res_mem, ParseFloat, 1)
	}

	var sum float64
	for _, v := range sqrts {
		sum += v
	}

	fmt.Printf("The final sum of square roots is %f\n", sum)
	os.Exit(0)
}
