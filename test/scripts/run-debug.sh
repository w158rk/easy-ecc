#!/bin/bash 

x=1 
while [ $x -le 10 ]
do 
    ../build-debug/bin/keytest 
    x=$(( $x + 1 ))
done