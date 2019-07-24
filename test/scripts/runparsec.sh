#!/bin/bash

PARSECDIR=../build
BINDIR=bin
runmode=$1
#type in (detmp pthreads threads-jemalloc dthreads)
type=$2
app=$3
data=$4 #(hamlet small medium large)
# ncpus=($5)
res=results/hotplug/$type
# echo "$runmode $app $type $data (${ncpus[@]})"
echo "$runmode $app $type $data"

#rm -rf $res
TIMEFORMAT=$'real %6R/tuser %6U/tsys %6S'

# import common functions for testing
source runcommon.sh

# import config file to run parsec programs
# source parsecruncfg.sh

############################################
#                                          #
# Main                                     #
#                                          # 
############################################

# if [ "$runmode" != "perfreport" ]; then
  # Disable all CPU cores
  # disableCPUs

  # Set cpu_nums and thrd_nums of $app
  # set_nthrds $app $type
# fi

last=1
# for ncpu in ${ncpus[*]}; do
  # if [ "$runmode" != "perfreport" ]; then
    # Find corresponding number of threads
    # locate_elem "${cpu_nums[*]}" "$ncpu";
    # nt=${thrd_nums[$?]}

    # Enable CPU cores
    # enableCPUs $last $nt
    # last=$nt
    # Get $run_args
    # get_run_args $app $data $PWD
  # fi

case $runmode in 
  "perfrecord"   )
    nreps=1
    mkdir -p perf_res
    cmd="perf record -g $PARSECDIR-$type/bin/$app $data $PWD && mv perf.data perf_res/$app-$type-$data-$ncpu.data 2>&1 | tee -a perf_res/$app-$type-$data-$ncpu.record"
    echo $cmd;;
  "perfreport"   )
    nreps=1
    cmd="perf report -i perf_res/$app-$type-$data-$ncpu.data --stdio --sort dso -g flat 2>&1 | tee -a perf_res/$app-$type-$data-$ncpu.report";;
  "test"    )
    nreps=10
    mkdir -p $res
    resfile=$res/$app-$type-${data}.$ncpus
    touch $resfile
    echo 'dataset='${data}', ncpu='$ncpu', nt='$nt		
    echo 'dataset='${data}', ncpu='$ncpu', nt='$nt, $n >> $resfile
    cmd="(eval time $PARSECDIR-$type/bin/$app $run_args 2>&1) 2>&1| tee -a $resfile";;
esac

for n in `seq $nreps`
do
  echo $n
  echo $cmd|awk '{run=$0; system(run)}'
  #echo $cmd
done
# done
