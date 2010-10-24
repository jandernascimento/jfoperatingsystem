#/bin/bash
#Generates the statistics
#By: Jander Nascimento
#Depends: gnuplot package

for algo in $(echo FirstFit BestFit WorstFit|cat); do
make clean && make ALGORITHM=$algo ; make ALGORITHM=$algo mem_shell ; make ALGORITHM=$algo test; make ALGORITHM=$algo graph
#gnuplot -e 'set term png; set ylabel "Amount of memory";set output "statistics/$ALGORITHM.png";set title "Data"; set autoscale; plot "statistics/$(ALGORITHM)_statistics.dat" with lines;'


done

gnuplot -e 'set term png; set output "statistics/merged.png"; set ylabel "% of Fragmentation";set title "Memory Allocator - Benchmark"; set autoscale; plot "statistics/FirstFit_statistics.dat" with lines,"statistics/BestFit_statistics.dat" with lines, "statistics/WorstFit_statistics.dat" with lines';

