#!/usr/bin/zsh

cd golang

go build cmd/parent/parent.go && \
go build cmd/child/child.go && \
./parent ../$1 $2

cd ..
