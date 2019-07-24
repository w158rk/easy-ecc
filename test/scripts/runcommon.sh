############################################
#  Common functions for testing benchmarks
############################################

######################################################
# Find given elem in the specified array
# $1: array
# $2: elem
######################################################
function locate_elem {
  local i=0
  local array=($1)
  local ncpu=$2
  local n=${#array[@]}
  echo "$n (${array[*]})"
  while [ $i -lt $n ]
  do
    if [ $ncpu == ${array[$i]} ]; then
      echo "$ncpu is element $i in (${array[*]})"
      break;
    fi
    let i++
  done
  if [ $i == $n ]; then
    echo "$2($n) is not in $array"
    exit 1;
  fi
  return $i;
}

############################################
# Disable all CPU cores
# $1: number of CPU cores
############################################
function disableCPUs {
  ncores=`cat /proc/cpuinfo | grep "cpu cores" | wc -l`
  for coreid in $(seq 1 $[$ncores-1])
  do
    echo 0 | sudo tee /sys/devices/system/cpu/cpu$coreid/online
  done
}

############################################
# Enable CPU cores which ID is from $1 to $2-1
# $1: number of CPU cores which have been online
# $2: total number of CPU cores to be enabled
############################################
function enableCPUs {
  for((coreid = $1; coreid<$2; coreid++))
  do
    echo 1 | sudo tee /sys/devices/system/cpu/cpu$coreid/online
  done
}
