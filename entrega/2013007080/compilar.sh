#!/bin/bash

gcc -g -c bibsimplex.c -o bibsimplex.o
gcc -g -c simplex.c -o main.o
gcc bibsimplex.o main.o -o executavel
