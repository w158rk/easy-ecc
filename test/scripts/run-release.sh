#!/bin/bash 

x=1 
while [ $x -le 10 ]
do 
    ../build-release/bin/keytest 
    x=$(( $x + 1 ))
done