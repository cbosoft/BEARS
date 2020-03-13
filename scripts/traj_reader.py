

def read_tsv_fast(path, *headings):
    if not headings:
        raise Exception('What data would you like? position, velocity, kinetic energy... ?')

    with open(path) as f:
        versionline = f.readline()
        print(versionline)

        n_and_geom = f.readline()
        __, n, __, L = n_and_geom.split()
        n = int(n)
        L = float(L)

        headerline = f.readline()
        log_headings = headerline.split()
        print(log_headings)
        for heading in headings:
            if heading not in log_headings and f'{heading}x' not in log_headings:
                raise Exception(f'Data field "{heading}" not understood.')

        rv = dict()
        for heading in headings:
            rv[heading] = list()

        done = False
        # TODO: how best to store the data as a function of time?
        while not done:
            for i in range(n):
                line = f.readline()
                if not line:
                    done = True
                    break

                values = line.split()
                vec = list()
                for value, heading in zip(values, log_headings):
                    if heading in headings:
                        rv[heading].append(value)
                    elif heading[:-1] in headings:
                        vec.append(value)
                        if heading[-1] != 'z':
                            rv[heading[:-1]].append(value)
                            vec = list()
    return rv


if __name__ == "__main__":
    data = read_tsv_fast('traj.tsv', 'position', 'kinetic_energy')
    print(data)
