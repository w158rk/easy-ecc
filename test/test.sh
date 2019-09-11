#!/bin/bash

BINDIR=bin
OUTDIR=./data
app=$1
version=$2
count=$3
note=$4
BUILDDIR=../build-analyze-$version-bit

# compile 
mkdir -p $BUILDDIR 
sh -c `cd $BUILDDIR && cmake -DCMAKE_BUILD_TYPE=Analyze-$version-bit ../ && make`

# run 
$BUILDDIR/$BINDIR/$app $count # run the program 
echo "run finished"

# generate data
mkdir -p $OUTDIR
gprof -b -A -p -q $BUILDDIR/$BINDIR/$app gmon.out > $OUTDIR/$app-data-$version-bit-$note
echo "output to" $OUTDIR/$app-data-$version-bit-$note
rm gmon.out