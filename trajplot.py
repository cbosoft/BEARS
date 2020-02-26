import sys

import numpy as np
from matplotlib import pyplot as plt
import yaml

#fig, axes = plt.subplots(nrows=3)

with open(sys.argv[1]) as yamlf:
    data = yaml.safe_load(yamlf)

# axes[0].plot(data)
# axes[1].plot(np.diff(data))
# axes[2].hist(np.diff(data), bins=100)
# plt.savefig('traj.pdf')

nballs = len(data[0]['balls'])
px_data = [list() for i in range(nballs)]
py_data = [list() for i in range(nballs)]
vx_data = [list() for i in range(nballs)]
vy_data = [list() for i in range(nballs)]
for step in data:
    for i, ball in enumerate(step['balls']):
        px_data[i].append(ball['position'][0])
        py_data[i].append(ball['position'][1])
        vx_data[i].append(ball['velocity'][0])
        vy_data[i].append(ball['velocity'][1])

nsteps = len(px_data[0])
for t in range(nsteps):
    plt.figure(figsize=(5,5))
    for i in range(nballs):
        px, py = px_data[i][t], py_data[i][t]
        vx, vy = vx_data[i][t], vy_data[i][t]
        plt.plot(px, py, 'o', color=f'C{i}')
        #plt.circles(x_data[i][t], y_data[i][t], s=1.0)
        plt.gca().add_patch(plt.Circle( (px, py), 0.5, color=f'C{i}'))
        plt.plot(  [px, px+vx], [py, py+vy], '-', color='black')
    plt.show()
    plt.close()
