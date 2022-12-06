import numpy as np

def histogram(data, counts, bin_edges, density=True):
    bin_edges = np.sort(bin_edges)
    new_counts = np.zeros(len(bin_edges)-1,dtype=int)
    print("searchsorted")
    ndcs = np.searchsorted(bin_edges, data)
    print("done")
    allowed_ndcs = np.where(np.logical_and(ndcs>0, ndcs<=len(new_counts)))
    ndcs = ndcs[allowed_ndcs]
    counts = counts[allowed_ndcs]
    for ndx, C in zip(ndcs, counts):
        new_counts[ndx-1] += C
    if density:
        dx = np.diff(bin_edges)
        all_counts = new_counts.sum()
        new_counts = new_counts.astype(float) / dx / all_counts

    return new_counts, bin_edges

def slow_histogram(data, counts, bin_edges, density=True):
    new_data = [ np.ones(int(c))*r for r, c in zip(data, counts) ]
    new_data = np.concatenate(new_data)

    return np.histogram(new_data,bins=bin_edges,density=density)
