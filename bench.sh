#!/usr/bin/bash

bench_fail() {
  echo "benchmark failed! $1"
  exit 1
}

# create required configurations of particles
make clean && make BEARS configgen || bench_fail "compile error"

numbers="10 100 1000 2000 3000 5000 8000 10000 20000 30000"
for n in $numbers; do
  ./configgen -n "$n" -o "$n.tsv" || bench_fail "config error"
  ./BEARS -i "$n.tsv" || bench_fail "runtime error"
done
