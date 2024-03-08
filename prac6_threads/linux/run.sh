#!/usr/bin/zsh

cd linux

gcc linux.c error.h mem.h scan.h -o linux -lm -lpthread && \
./linux ../$1 $2

cd ..
