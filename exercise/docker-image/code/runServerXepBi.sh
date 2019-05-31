#!/bin/sh

make clean
make all
./server.o 4 10 # max client: 4 - arrSize: 10
