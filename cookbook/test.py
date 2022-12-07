import numpy as np

from cQuadTree import QuadTree, histogram
#from cQuadTree.utils import histogram2
from scipy.spatial.distance import pdist
import matplotlib.pyplot as pl

N = 10_000

pos = np.random.rand(N,2)
lpos = pos.tolist()

print("building tree")
T = QuadTree(lpos)
print("done")
#help(T)

#T.get_distances_to((0.5,0.5),.2,True,T)
#T.get_distances_to((0.5,0.5),.2,True,T)
print("querying tree")
#dists = np.array(T.get_distances_to_points(lpos, 0.1, True, T))
dists = np.array(T.get_distances_to_points(lpos, 0.2))
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

pl.yscale('log')
pl.xscale('log')

pl.show()




