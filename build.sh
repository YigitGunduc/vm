#!/bin/sh
#set -xe

OUTPUT="vm.out"
FLAGS="-Wall -o $OUTPUT"

cc main.c src/*.c $FLAGS
./$OUTPUT $1
rm ./$OUTPUT
