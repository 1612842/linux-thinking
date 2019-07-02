#!/bin/bash


find -type f \( -name "*.cpp" -o -name "*.c" -o -name "*.h" \) -exec awk 'BEGIN {count = 0;}
{count++;}  
END {print count/1000, "KLOC";}' {} +
