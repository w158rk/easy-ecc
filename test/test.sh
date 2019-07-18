#!/bin/sh 

# Author: Wang Ruikai 
# Script follows here 

echo "encrypt-decrypt test"
perf record -e cpu-clock,faults ../build/bin/keytest
# perf report 
# perf report --stdio --sort comm,dso
# perf report --stdio --dsos=keytest,libc-2.27.so
perf annotate --stdio --dsos=keytest