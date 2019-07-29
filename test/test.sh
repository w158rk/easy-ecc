#!/bin/bash

BUILDDIR=../build-analyze
BINDIR=bin
OUTDIR=./data
app=$1
count=$2
note=$3

# compile 
mkdir -p $BUILDDIR 
sh -c "cd $BUILDDIR && cmake -DCMAKE_BUILD_TYPE=Analyze ../ && make"

# run 
$BUILDDIR/$BINDIR/$app $count # run the program 
echo "run finished"

# generate data
mkdir -p $OUTDIR
gprof -b -A -p -q $BUILDDIR/$BINDIR/$app gmon.out > $OUTDIR/$app-data-$note
echo "output to" $OUTDIR/$app-data-$note
rm gmon.out