#!/bin/bash

## This is to run the first program main.cpp

if [ -e "bonus" ]; then
    ./bonus $1 $2 $3 $4 $5 $6
else
    echo "First run compile.sh to compile the code"
fi