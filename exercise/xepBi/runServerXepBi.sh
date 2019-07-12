#!/bin/sh

make clean
make all
./server.o 4 200 # max client: 4 - arrSize: 10
