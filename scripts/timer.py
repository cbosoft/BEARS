#!/usr/bin/env python
import sys
from matplotlib import pyplot as plt

def read_output(path):
    with open(path) as f:
        lines = f.readlines()
    return [float(line.split()[3]) for line in lines[8:]]

for path in sys.argv[1:]:
    plt.plot(read_output(path), label=path)

plt.legend()
plt.show()
