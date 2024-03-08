#!/usr/bin/zsh

cd linux

gcc linux_parent.c error.h mem.h scan.h -o parent -lm && \
gcc linux_child.c error.h mem.h scan.h -o child -lm && \
./parent ../$1 $2

cd ..
