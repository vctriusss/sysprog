package main

import (
	"fmt"
	"io"
	"os"
	"os/exec"
	// "bufio"
	. "prac5/pkg/utils"
	"strconv"
)


func getNWrite(i int) int {
	if i == N-1 {
		return nLast
	}
	return nEach
}


func startSubProcess(child *exec.Cmd) io.ReadCloser {
	childErr, err := child.StderrPipe()
	CheckFileErr(err, "child.StderrPipe")
	
	if child.Start() != nil {
		Err("Failed to start subprocess: %v", err)
	}
	
	childErrStr, err := io.ReadAll(childErr)
	if err != nil {
		Err("Can't read child process stderr: %v", err)
	}
	if len(childErrStr) > 0 {
		Err(string(childErrStr))
	}
	return childErr
}


var (
	N     int
	nEach int
	nLast int
	filename string
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

	children := make([]*exec.Cmd, N)

	for i := 0; i < N; i++ {
		nWrite := getNWrite(i)

		fname := fmt.Sprintf("../files/file%d.txt", i+1)
		f, err := os.Create(fname)
		CheckFileErr(err, fname)

		WriteVals(f, nums[i*nEach:i*nEach+nWrite], false)

		children[i] = exec.Command("./child", fname, strconv.Itoa(nWrite))
	}
	var sum float64

	for _, child := range children {
		childErr := startSubProcess(child)
		defer childErr.Close()
	}

	// bufio.NewReader(os.Stdin).ReadBytes('\n') 

	for i, child := range children {
		if child.Wait() != nil {
			Err("Child process %d finished unsuccessfully: %v", i+1, err)
		}
	}

	for i := 0; i < N; i++ {
		fname := fmt.Sprintf("../files/file%d.txt", i+1)
		childFile, err := os.Open(fname)
		CheckFileErr(err, fname)

		sqrts := ScanVals[float64](childFile, ParseFloat, getNWrite(i))

		sum += sqrts[0]
	}

	fmt.Printf("The final sum of square roots is %f\n", sum)
	os.Exit(0)
}
