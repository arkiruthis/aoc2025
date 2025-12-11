#!/usr/bin/env bash

if [ $# -eq 0 ]; then
    echo "Usage: $0 <source_file.cpp>"
    exit 1
fi

SOURCE_FILE="$1"

if [ ! -f "$SOURCE_FILE" ]; then
    echo "Error: File '$SOURCE_FILE' not found"
    exit 1
fi

clang++ -std=c++20 -O3 -Wall "$SOURCE_FILE" -o ./main

if [ $? -eq 0 ]; then
    echo "Successfully compiled $SOURCE_FILE to ./main"
else
    echo "Compilation failed"
    exit 1
fi