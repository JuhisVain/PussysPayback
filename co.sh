#!/bin/bash

export LD_LIBRARY_PATH=./blt/
gcc src/main.c src/world.c blt/libBearLibTerminal.so
