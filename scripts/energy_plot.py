import sys

import numpy as np
from matplotlib import pyplot as plt

path = 'traj.tsv'
if len(sys.argv) > 1:
    path = sys.argv[1]

with open(path) as f:
    lines = f.readlines()

time = list()
av_kinetic_energy = list()
this_step_energy = list()
for line in lines:
    if line.startswith('BEARS') or line.startswith('id'):
        continue
    elif line.startswith('n:'):
        __, n, __, L = line.split()
        n = int(n)
        L = float(L)
    elif line.startswith('t='):
        __, t, aid, __, bid = line.split()
        t = float(t)
        time.append(t)
        if this_step_energy:
            av_kinetic_energy.append(np.average(this_step_energy))
            this_step_energy = list()
        aid = int(aid)
        bid = int(bid)
        if aid > n: aid = -1
        if bid > n: bid = -1
    else:
        ball_info = line.split()
        kinetic_energy = float(ball_info[-1])
        this_step_energy.append(kinetic_energy)
if this_step_energy:
    av_kinetic_energy.append(np.average(this_step_energy))


plt.plot(time, av_kinetic_energy)
plt.xlabel('Time, $t$')
plt.ylabel('Average Kinetic Energy, $\overline{E_K}$')
plt.ylim(bottom=0.0, top=max(av_kinetic_energy)*1.1)
plt.savefig('kinetic_energy.pdf')
