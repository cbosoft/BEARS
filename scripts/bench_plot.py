import math as m

from mpl_toolkits.mplot3d import Axes3D
from matplotlib import pyplot as plt

def read_bench():
    with open("benchmark.csv") as f:
        lines = f.readlines()
    data = [line.split(',') for line in lines]
    data = list(zip(*data))
    branches = set(data[1])
    rv = {b:{
        'time':list(),
        'branch':b,
        'nthreads':list(),
        'nparticles':list(),
        'time_to_update':list()
        } for b in branches}
    data = list(zip(*data))
    for time, branch, nthreads, nparticles, time_to_update in data:
        rv[branch]['time'].append(float(time))
        rv[branch]['nthreads'].append(int(nthreads))
        rv[branch]['nparticles'].append(int(nparticles))
        rv[branch]['time_to_update'].append(m.log(float(time_to_update)))
    return rv

if __name__ == "__main__":
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    data = read_bench()

    for branch in data:
        plt.plot(data[branch]['nparticles'],
                data[branch]['nthreads'],
                data[branch]['time_to_update'], 'o')
    #plt.xscale('log')
    #plt.yscale('log')
    ax.set_ylabel('number of threads')
    ax.set_xlabel('number of particles')
    ax.set_zlabel('log(time_to_update)')
    plt.show()

