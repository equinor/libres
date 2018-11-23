from collections import OrderedDict

class GravObs(OrderedDict):

    def __init__(self, filename):
        super(GravObs, self).__init__()
        with open(filename) as f:
            for line in f.readlines():
                tmp = line.split( )
                name = tmp[0]
                value = float(tmp[1])
                std = float(tmp[2])

            self[name] = (value , std)


    def chi2(self, sim_data):
        c2 = 0
        for name in self.keys():
            value,std = self[name]
            x = (sim_data[ name ] - value) / std

            c2 += x*x

        return c2
