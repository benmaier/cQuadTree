import matplotlib as mpl
import matplotlib.pyplot as pl
from matplotlib.collections import PatchCollection
from matplotlib.patches import Rectangle

def plot_boxes(
               list_of_extents,
               ax=None,
               figsize=(6,6),
               facecolor='k',
               edgecolor='k',
               alpha=0.05,
           ):
    """
    Plot a graphical representation of the tree as boxes on a matplotlib.Axes.

    Use with data obtained from :func:`cQuadTree.utils.get_points_and_boxes`.
    """
    if ax is None:
        fig, ax = pl.subplots(1,1,figsize=figsize)

    pl.sca(ax)
    pl.axis('square')

    rects = []
    for E in list_of_extents:
        rect = Rectangle((E.l(), E.b()), E.w(), E.h())
        rects.append(rect)

    patches = PatchCollection(rects,
                              alpha = alpha,
                              edgecolor=edgecolor,
                              facecolor=facecolor,
                              #zorder=-1000,
                          )

    ax.add_collection(patches)

    ax.get_figure().tight_layout()

    return ax

def plot_points(
               list_of_points,
               ax=None,
               figsize=(6,6),
               marker='x',
               color='k',
               **kwargs,
           ):
    """
    Plot a graphical representation of the points that reside on the tree leaves on a matplotlib.Axes.

    Use with data obtained from :func:`cQuadTree.utils.get_points_and_boxes`.
    """

    if ax is None:
        fig, ax = pl.subplots(1,1,figsize=figsize)

    pl.sca(ax)
    pl.axis('square')

    x, y = zip(*[(P.x, P.y) for P in list_of_points])

    ax.plot(x, y, ls='None', marker=marker, color=color, **kwargs)

    ax.get_figure().tight_layout()

    return ax

def plot_box_tree(list_of_points,list_of_boxes,ax=None,box_kwargs={},point_kwargs={}):
    """
    Plot a graphical representation of the tree as boxes and points on a matplotlib.Axes.

    Use with data obtained from :func:`cQuadTree.utils.get_points_and_boxes`.
    """
    ax = plot_points(list_of_points, ax=ax, **point_kwargs)
    ax = plot_boxes(list_of_boxes, ax=ax, **point_kwargs)
    return ax

