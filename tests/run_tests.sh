#!/bin/bash

echo "BUILDING"

make clean
make

echo "RUNNING EXECUTABLES:"

for exe in *.exe; do
    ./$exe
done

echo "STARTING OPENOCD IN BACKGROUND:"
openocd -f stm32f407g.cfg &
PID=$!

echo "PERFORMING TESTS:"

success_count=0
fail_count=0
result=0

for expect in *.expect; do
    chmod a+x $expect
    ./$expect

    result=$?

    echo 
    echo "------------------------------------------------------------"
    if [ $result -eq 0 ]
    then
	success_count=$((success_count+1))
	echo $expect SUCCESS
    else
	fail_count=$((fail_count+1))
	echo $expect FAILED
    fi
    echo "------------------------------------------------------------"
done

echo Tests passed: $success_count
echo Tests failed: $fail_count

kill -9 $PID


if [ $fail_count -gt 0 ]
then
    exit 1
fi
