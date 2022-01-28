#!/bin/bash

export LD_LIBRARY_PATH=./blt/
gcc -Wall src/main.c src/world.c src/cat.c src/game.c blt/libBearLibTerminal.so
