import numpy as np

def histogram(data, counts, bin_edges, density=True):
    """
    Returns a histogram from distance count data
    received from a tree.

    Parameters
    ==========
    data : numpy.ndarray of float
        Distances
    counts : numpy.ndarray of int
        Corresponding counts of distances in ``data``.
    bin_edges : numpy.ndarray
        Edges of bins for which the histogram should be computed
    density : boolean, default = True
        Whether or not to make the histogram a probability density

    Returns
    =======
    hist : numpy.ndarray
        Either count of data in bins, or pdf, will have length
        ``len(bin_edges)-1``.
    bin_edges : numpy.ndarray
        The used bin edges
    """
    bin_edges = np.sort(bin_edges)
    new_counts = np.zeros(len(bin_edges)-1,dtype=int)
    ndcs = np.searchsorted(bin_edges, data)
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

def get_points_and_boxes(quadtree):
    """
    Returns two lists, one filled with "Extent" objects
    representing the boxes of the tree that are occupied,
    the other one contains the points that are located at
    the leaf nodes

    Parameters
    ==========
    quadtree : :class:`_cQuadTree.QuadTree`
        Self-explanatory, no?

    Returns
    =======
    points : list of :class:`_cQuadTree.Point`
        Points located at the leaves ot the tree
    boxes : list of :class:`_cQuadTree.Extent`
        The boxes that internal tree nodes represent
    """

    points = []
    boxes = []
    if quadtree.is_leaf():
        points.append(quadtree.this_pos)
    boxes.append(quadtree.geom)
    for tree in quadtree.get_subtrees():
        _points, _boxes = get_points_and_boxes(tree)
        points.extend(_points)
        boxes.extend(_boxes)

    return points, boxes


if __name__=="__main__":

    from cQuadTree import QuadTree
    from cQuadTree.plot import plot_boxes, plot_points, plot_box_tree
    import matplotlib.pyplot as pl

    points = np.random.rand(100,2).tolist()
    T = QuadTree(points)
    points, boxes = get_points_and_boxes(T)
    plot_box_tree(points, boxes)
    pl.show()
