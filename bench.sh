#!/usr/bin/bash

# create required configurations of particles
make clean && make BEARS configgen || exit

numbers="10 100 1000 2000 3000 5000 8000 10000 20000 30000"
for n in $numbers; do
  ./configgen -n "$n" -o "$n.tsv"
  ./BEARS -i "$n.tsv"
done
