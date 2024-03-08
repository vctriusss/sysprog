#!/usr/bin/zsh

cd linux

gcc linux.c parent_pipes.h parent_shm.h -o linux -lm -lpthread && \
gcc child_pipes.c -o child_pipes -lm && \
gcc child_shm.c -o child_shm -lm -lpthread && \
./linux ../$1 $2 $3

cd ..
