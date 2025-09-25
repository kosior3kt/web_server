#!/usr/bin/env bash

gcc test.c -g -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -o test && ./test
#gcc main.c -g -o prog && ./prog

