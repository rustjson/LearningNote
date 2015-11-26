#!/bin/bash
 
#############################################################################
#
# This script aims to test/prove that you can append to a single file from
# multiple processes with buffers up to a certain size, without causing one
# process' output to corrupt the other's.
#
# The script takes one parameter, the length of the buffer. It then creates
# 20 worker processes which each write 50 lines of the specified buffer
# size to the same file. When all processes are done outputting, it tests
# the output file to ensure it is in the correct format.
#
#############################################################################
 
NUM_WORKERS=20
LINES_PER_WORKER=50
OUTPUT_FILE=/tmp/out.tmp
 
# each worker will output $LINES_PER_WORKER lines to the output file
run_worker() {
    worker_num=$1
    buf_len=$2
 
    # Each line will be a specific character, multiplied by the line length.
    # The character changes based on the worker number.
    filler_len=$((${buf_len}-1)) # -1 -> leave room for \n
    filler_char=$(printf \\$(printf '%03o' $(($worker_num+64))))
    line=`for i in $(seq 1 $filler_len);do echo -n $filler_char;done`
    for i in $(seq 1 $LINES_PER_WORKER)
    do
        echo $line >> $OUTPUT_FILE
    done
}
 
if [ "$1" = "worker" ]; then
    run_worker $2 $3
    exit
fi
 
buf_len=$1
if [ "$buf_len" = "" ]; then
    echo "Buffer length not specified, defaulting to 4096"
    buf_len=4096
fi
 
rm -f $OUTPUT_FILE
 
echo Launching $NUM_WORKERS worker processes
for i in $(seq 1 $NUM_WORKERS)
do
    $0 worker $i $buf_len &
    pids[$i]=${!}
done
 
echo Each line will be $buf_len characters long
echo Waiting for processes to exit
for i in $(seq 1 $NUM_WORKERS)
do
    wait ${pids[$i]}
done
 
# Now we want to test the output file. Each line should be the same letter
# repeated buf_len-1 times (remember the \n takes up one byte). If we had
# workers writing over eachother's lines, then there will be mixed characters
# and/or longer/shorter lines.
 
echo Testing output file
 
# Make sure the file is the right size (ensures processes didn't write over
# eachother's lines)
expected_file_size=$(($NUM_WORKERS * $LINES_PER_WORKER * $buf_len))
actual_file_size=`cat $OUTPUT_FILE | wc -c`
if [ "$expected_file_size" -ne "$actual_file_size" ]; then
    echo Expected file size of $expected_file_size, but got $actual_file_size
else
  
    # File size is OK, test the actual content
 
    # Only use newer versions of grep because older ones are way too slow with
    # backreferences
    [[ $(grep --version) =~ [^[:digit:]]*([[:digit:]]+)\.([[:digit:]]+) ]]
    grep_ver="${BASH_REMATCH[1]}${BASH_REMATCH[2]}"
    if [ "$grep_ver" -ge "216" ]; then
        num_lines=$(grep -v "^\(.\)\1\{$((${buf_len}-2))\}$" $OUTPUT_FILE | wc -l)
    else
        # Scan line by line in bash, which isn't that speedy, but is good enough
        # Note: Doesn't work on cygwin for lines < 255
        line_length=$((${buf_len}-1))
        num_lines=0
        for line in `cat $OUTPUT_FILE`
        do
            if ! [[ $line =~ ^${line:0:1}{$line_length}$ ]]; then
                num_lines=$(($num_lines+1))
            fi;
            echo -n .
        done
        echo
    fi
 
    if [ "$num_lines" -gt "0" ]; then
        echo "Found $num_lines instances of corrupted lines"
        else
        echo "All's good! The output file had no corrupted lines. $size"
    fi
fi
 
#rm -f $OUTPUT_FILE
