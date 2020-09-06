#!/bin/bash

echo "BUILDING"

make clean
make

echo "RUNNING EXECUTABLES:"

for exe in *.exe; do
    ./$exe
done

echo "PERFORMING TESTS:"

for expect in *.expect; do
    chmod a+x $expect
    ./$expect
done
