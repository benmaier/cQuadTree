import numpy as np

from cQuadTree import QuadTree, histogram
#from cQuadTree.utils import histogram2
from scipy.spatial.distance import pdist
import matplotlib.pyplot as pl

N = 3_000

N_gauss = 4
sigma = 0.04
positions = []
for gaus in range(N_gauss):
    this_pos = sigma*np.random.randn(N,2)
    pos = np.random.rand(2)
    this_pos += pos[None,:]
    positions.extend(this_pos.tolist())
pos = np.array(positions)
lpos = positions

print("building tree")
T = QuadTree(lpos)
print("done")
#help(T)

#T.get_distances_to((0.5,0.5),.2,True,T)
#T.get_distances_to((0.5,0.5),.2,True,T)
print("querying tree")
dists = np.array(T.get_distances_to_points(lpos, 0.3, True, T))
print("done")
#dists = np.array(T.get_distances_to_points(T, lpos))
dists, counts = dists[:,0], dists[:,1]
print(dists, counts)

bin_edges = np.logspace(-10,1/2,101,base=2)
x = np.sqrt(bin_edges[1:]*bin_edges[:-1])

print("building histogram")
density, _ = histogram(dists, counts, bin_edges, density=True)
print(density)
print("done")
pl.plot(x, density)
#print("building histogram")
#density, _ = histogram2(dists, counts, bin_edges, density=True)
#print("done")
#pl.plot(x, density)

print("computing pairwise distances")
density, _ = np.histogram(pdist(pos), bins=bin_edges,density=True)
print("done")
pl.plot(x, density)

pl.xscale('log')
pl.yscale('log')

pl.show()




