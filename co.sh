#!/bin/bash

export LD_LIBRARY_PATH=./blt/
gcc -Wall -O3 src/main.c src/world.c src/cat.c src/game.c blt/libBearLibTerminal.so
