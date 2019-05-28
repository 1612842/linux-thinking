#!/bin/bash


for ((i=1;i<=10;i++))
do
gnome-terminal -e 'bash -c "./client.o auto; exec bash"'
done
