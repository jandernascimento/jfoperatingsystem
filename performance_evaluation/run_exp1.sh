#! /bin/bash

N='100 1000 10000 100000 1000000 10000000'
R=5
P='1 2 3 4'

for n in $N; 
do
  for r in `seq 1 $R`;
  do
    ./sort_seq -n $n -r -s 1
  done
done

for p in $P;
do
  for n in $N;
  do
    for r in `seq 1 $R`;
    do
      ./sort_par -n $n -r -s 1
    done
  done
done 
