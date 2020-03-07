import sys

import numpy as np
from matplotlib import pyplot as plt
import yaml

path = 'traj.yaml'
if len(sys.argv) > 1:
    path = sys.argv[1]

with open(path) as yamlf:
    data = yaml.safe_load(yamlf)

av_kinetic_energy = list()
time = list()
n = len(data[0]['balls'])
for step in data:
    ke = 0.0
    for i, ball in enumerate(step['balls']):
        print(ball)
        ke += ball['kinetic_energy']
    av_kinetic_energy.append(ke/n)
    time.append(step['time'])

plt.plot(time, av_kinetic_energy)
plt.xlabel('Time, $t$')
plt.ylabel('Average Kinetic Energy, $\overline{E_K}$')
plt.savefig('kinetic_energy.pdf')
