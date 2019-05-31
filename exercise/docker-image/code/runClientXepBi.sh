#!/bin/bash

#run 4 client auto get
for ((i=1;i<=4;i++))
do
gnome-terminal -e 'bash -c "./client.o auto; exec bash"'
done
