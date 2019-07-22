#!/bin/sh 

# Author: Wang Ruikai 

# before your execution 

# sudo sysctl kernel/perf_event_paranoid=-1   
# sudo sh -c "echo 0 > /proc/sys/kernel/kptr_restrict"     
# sudo mount -o remount,mode=755 /sys/kernel/debug
# sudo mount -o remount,mode=755 /sys/kernel/debug/tracing

# Script follows here 

echo "encrypt-decrypt test"
# perf stat -B -e cpu-clock,cache-references,cache-misses,cycles,instructions,branches,faults,migrations ../build/bin/keytest
# perf stat -d --repeat 10 -e cpu-clock,faults,task-clock ../build/bin/keytest
perf stat -g -e cpu-clock,faults,task-clock ./run.sh
perf record -g -e cpu-clock,faults,task-clock ./run.sh
perf report 
# perf report --stdio --sort comm,dso
# perf report --stdio --dsos=keytest,libc-2.27.so
# perf annotate --stdio --dsos=keytest
