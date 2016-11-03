#!/bin/bash

## Compile our two program files
g++ -std=c++11 -g -Ofast -march=native -o mdp main1.cpp state.cpp