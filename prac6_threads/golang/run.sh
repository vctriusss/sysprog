#!/usr/bin/zsh

cd golang

go build cmd/main.go && \
./main ../$1 $2

cd ..
