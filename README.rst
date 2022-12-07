cQuadTree
=========

Infinite jest with your favorite space-dividing trees. Fork from this
C++-codebase: https://github.com/benmaier/BarnesHutTree/

-  repository: https://github.com/benmaier/cQuadTree/

.. code:: python

   >>> from cQuadTree import QuadTree
   >>> points = [
   ...             (1.0, 2.0),
   ...             (0.1, 10.0),
   ...             (0.5, 7.4),
   ...             (6.0, 0.7),
   ...          ]
   >>> T = QuadTree(points)
   >>> T
   QuadTree(
       geom=Extent(left=0.1,bottom=0.7,width=9.3,height=9.3),
       current_data_quadrant=-1,
       is_leaf=False,
       number_of_contained_points=4,
       center_of_mass=Point(x=1.9,y=5.025),
       total_mass=4,
       total_mass_position=Point(x=7.6,y=20.1),
       number_of_occupied_subtrees=3
   )
   >>> print(T)
   +- CM = 1.9, 5.025; M = 4; n = 4
   | +- (nw) CM = 0.3, 8.7; M = 2; n = 2
   | | +- (nw) 1 (0.1, 10)
   | | +- (sw) 2 (0.5, 7.4)
   | +- (se) 3 (6, 0.7)
   | +- (sw) 0 (1, 2)

Install
-------

Your machine needs to have a C++11-compatible compiler installed.

.. code:: bash

   pip install cQuadTree

So far, the package's functionality was tested on Mac OS X and CentOS
only.

Dependencies
------------

``cQuadTree`` directly depends on the following packages which will be
installed by ``pip`` during the installation process

-  ``numpy>=1.20``

Documentation
-------------

Build the tree
~~~~~~~~~~~~~~

.. code:: python

   import numpy as np
   from cQuadTree import QuadTree
   points = np.random.rand(100,2).tolist()
   T = QuadTree(points)

Explore the tree recursively
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

As an example, here's a recursive function that collects all internal
node boxes and leaf's points

.. code:: python

   def get_points_and_boxes(quadtree):
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

Compute the force on a point
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code:: python

   >>> T.compute_force(point=(0.,0.001),theta=1.0) # default is theta=0.5
   (0.117681690892212, 0.20856460584929215)

Get all distances to a point
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Note that per default, distances of value zero will be disregarded. If
you want to include those, set ``ignore_zero_distance=False`` in the
function call.

.. code:: python

   >>> T.get_distances_to((0.,0),theta=1) # default is theta=0.2
   [(8.705170877128145, 2), (6.040695324215583, 1), (2.23606797749979, 1)]

The first number is the distance to the query point, the second is the
number of points that lie at this approximate distance to the query
point.

Get all distances between pairs of points in a list and points in the tree
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code:: python

   >>> T.get_distances_to_points(points,theta=1)
   [(6.7364679172397155, 2), (5.166236541235796, 1), (2.630589287593181, 1), (11.013627921806693, 1), (8.050465825031493, 1), (2.630589287593181, 1), (8.668333173107735, 1), (5.4230987451825, 1), (9.822932352408825, 2), (5.166236541235796, 1)]

Get all pairwise distances between points in the tree
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code:: python

   >>> T.get_pairwise_distances(theta=1.0)
   [(2.630589287593181, 1), (11.013627921806693, 1), (8.050465825031493, 1), (2.630589287593181, 1), (8.668333173107735, 1), (5.4230987451825, 1), (9.822932352408825, 2), (5.166236541235796, 1), (6.7364679172397155, 2), (5.166236541235796, 1)]

Build a distance histogram from distance counts
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code:: python

   from cQuadTree import histogram
   dists, counts = zip(*T.get_pairwise_distances(theta=1.0))
   bin_edges = np.logspace(-4,1/2,101,base=2)
   pdf, _ = histogram(dists, counts, bin_edges)

Plot tree as boxes and points
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code:: python

   from cQuadTree import get_points_and_boxes
   from cQuadTree.plot import plot_box_tree

   plot_box_tree(*get_points_and_boxes(T))

.. image:: https://github.com/benmaier/cQuadTree/blob/main/img/boxtree.png?raw=true
   :alt: Box representation of tree

Docstrings
~~~~~~~~~~

QuadTree
^^^^^^^^

.. code:: python

   Help on class QuadTree in module _cQuadTree:

   class QuadTree(pybind11_builtins.pybind11_object)
    |  A QuadTree.
    |
    |  Method resolution order:
    |      QuadTree
    |      pybind11_builtins.pybind11_object
    |      builtins.object
    |
    |  Methods defined here:
    |
    |  __init__(...)
    |      __init__(*args, **kwargs)
    |      Overloaded function.
    |
    |      1. __init__(self: _cQuadTree.QuadTree) -> None
    |
    |      Initialize an empty tree.
    |
    |      2. __init__(self: _cQuadTree.QuadTree, position_pairs: List[Tuple[float, float]], force_square: bool = True) -> None
    |
    |      Initialize a tree given a list of positions.
    |
    |      3. __init__(self: _cQuadTree.QuadTree, position_pairs: List[Tuple[float, float]], masses: List[float], force_square: bool = True) -> None
    |
    |      Initialize a tree given a list of positions and a list of corresponding masses.
    |
    |  __repr__(...)
    |      __repr__(self: _cQuadTree.QuadTree) -> str
    |
    |      Get string representation of object
    |
    |  __str__(...)
    |      __str__(self: _cQuadTree.QuadTree) -> str
    |
    |      Get a string representation of the full tree
    |
    |  compute_force(...)
    |      compute_force(self: _cQuadTree.QuadTree, point: Tuple[float, float], theta: float = 0.5) -> Tuple[float, float]
    |
    |
    |      Compute the force on a single point using the Barnes-Hut-Algorithm
    |      with cutoff parameter :math:`\theta`.
    |
    |      Parameters
    |      ----------
    |      point : 2-tuple of float
    |          Point in plane on which to compute the total force
    |      theta : float, default = 0.5
    |          If the distance between the point and the current internal node's
    |          center of mass is smaller than :math:`\theta` times the diameter
    |          of the internal node's extent (box), the algorithm will treat
    |          all children of this node as a giant point mass located at the
    |          center of mass of this internal node.
    |
    |      Returns
    |      -------
    |      force : 2-tuple of float
    |          Evaluated force vector
    |
    |  get_distances_to(...)
    |      get_distances_to(self: _cQuadTree.QuadTree, point: Tuple[float, float], theta: float = 0.2, ignore_zero_distance: bool = True, tree: _cQuadTree.QuadTree = None) -> List[Tuple[float, int]]
    |
    |
    |      Compute distances of point masses and mass clusters to a single point
    |      using the Barnes-Hut-Algorithm with cutoff parameter :math:`\theta`.
    |
    |      Parameters
    |      ----------
    |      point : 2-tuple of float
    |          Points in the plane to which to measure the distances
    |      theta : float, default = 0.2
    |          If the distance between the point and the current internal node's
    |          center of mass is smaller than :math:`\theta` times the diameter
    |          of the internal node's extent (box), the algorithm will treat
    |          all children of this node as a giant point mass located at the
    |          center of mass of this internal node.
    |      ignore_zero_distance : bool, default = True
    |          If the distance is zero, do or do not include this result in
    |          the ``distance_counts``-list.
    |
    |      Returns
    |      -------
    |      distance_counts : list of 2-tuple of double, int
    |          An item of this list is a distance-count pair,
    |          the first entry of the tuple containing a distance
    |          to the query point and the second entry being the
    |          number of points that lie at that approximate distance
    |          to the query point, such that it will look like this
    |
    |          .. code:: python
    |
    |              [
    |                  (0.2, 1),
    |                  (0.1, 1),
    |                  (1.5, 32),
    |                  ...
    |              ]
    |
    |  get_distances_to_points(...)
    |      get_distances_to_points(self: _cQuadTree.QuadTree, points: List[Tuple[float, float]], theta: float = 0.2, ignore_zero_distance: bool = True, tree: _cQuadTree.QuadTree = None) -> List[Tuple[float, int]]
    |
    |
    |      Compute distances of point masses and mass clusters to a list of points
    |      using the Barnes-Hut-Algorithm with cutoff parameter :math:`\theta`.
    |
    |      Parameters
    |      ----------
    |      points : 2-tuple of float
    |          List of points in the plane to which to measure the distances
    |      theta : float, default = 0.2
    |          If the distance between the point and the current internal node's
    |          center of mass is smaller than :math:`\theta` times the diameter
    |          of the internal node's extent (box), the algorithm will treat
    |          all children of this node as a giant point mass located at the
    |          center of mass of this internal node.
    |      ignore_zero_distance : bool, default = True
    |          If the distance is zero, do or do not include this result in
    |          the ``distance_counts``-list.
    |
    |      Returns
    |      -------
    |      distance_counts : list of 2-tuple of double, int
    |          An item of this list is a distance-count pair,
    |          the first entry of the tuple containing a distance
    |          to the query point and the second entry being the
    |          number of points that lie at that approximate distance
    |          to the query point, such that it will look like this
    |
    |          .. code:: python
    |
    |              [
    |                  (0.2, 1),
    |                  (0.1, 1),
    |                  (1.5, 32),
    |                  ...
    |              ]
    |
    |  get_pairwise_distances(...)
    |      get_pairwise_distances(self: _cQuadTree.QuadTree, theta: float = 0.2, ignore_zero_distance: bool = True) ->
   List[Tuple[float, int]]
    |
    |
    |      Compute distances between pairs of points and point clusters
    |      of a tree using the Barnes-Hut-Algorithm with cutoff parameter
    |      :math:`\theta`.
    |
    |      Iterates over points by querying the tree recursively, which
    |      might take longer than simply externally iterating over a list of points
    |      if they're known.
    |
    |      Parameters
    |      ----------
    |      theta : float, default = 0.2
    |          If the distance between the point and the current internal node's
    |          center of mass is smaller than :math:`\theta` times the diameter
    |          of the internal node's extent (box), the algorithm will treat
    |          all children of this node as a giant point mass located at the
    |          center of mass of this internal node.
    |      ignore_zero_distance : bool, default = True
    |          If the distance is zero, do or do not include this result in
    |          the ``distance_counts``-list.
    |
    |      Returns
    |      -------
    |      distance_counts : list of 2-tuple of double, int
    |          An item of this list is a distance-count pair,
    |          the first entry of the tuple containing a distance
    |          to the query point and the second entry being the
    |          number of points that lie at that approximate distance
    |          to the query point, such that it will look like this
    |
    |          .. code:: python
    |
    |              [
    |                  (0.2, 1),
    |                  (0.1, 1),
    |                  (1.5, 32),
    |                  ...
    |              ]
    |
    |  get_subtree(...)
    |      get_subtree(self: _cQuadTree.QuadTree, arg0: int) -> _cQuadTree.QuadTree
    |
    |      Get subtree 0<=i<=3.
    |
    |  get_subtrees(...)
    |      get_subtrees(self: _cQuadTree.QuadTree) -> List[_cQuadTree.QuadTree]
    |
    |      Get a list of all of this node's children that contain data.
    |
    |  is_leaf(...)
    |      is_leaf(self: _cQuadTree.QuadTree) -> bool
    |
    |      Whether or not this node is a leaf.
    |
    |  ----------------------------------------------------------------------
    |  Data descriptors defined here:
    |
    |  center_of_mass
    |      Mass-weighted mean position of all points contained in this internal node.
    |
    |  current_data_quadrant
    |      Quadrant of the parent geometry the data of this tree resides in.
    |
    |  geom
    |      Extent of box this tree represents.
    |
    |  number_of_contained_points
    |      Number of points contained in this internal node.
    |
    |  parent
    |      The parent of this internal node.
    |
    |  this_id
    |      Data index of the point contained in this leaf.
    |
    |  this_mass
    |      Mass the point contained in this leaf.
    |
    |  this_pos
    |      Position of the point contained in this leaf.
    |
    |  total_mass
    |      Total mass of all points contained in this internal node.
    |
    |  total_mass_position
    |      Sum of product of mass and position of all points contained in this internal node.
    |

Extent
^^^^^^

.. code:: python

   class Extent(pybind11_builtins.pybind11_object)
    |  A rectangular geometry.
    |
    |  Method resolution order:
    |      Extent
    |      pybind11_builtins.pybind11_object
    |      builtins.object
    |
    |  Methods defined here:
    |
    |  __init__(...)
    |      __init__(*args, **kwargs)
    |      Overloaded function.
    |
    |      1. __init__(self: _cQuadTree.Extent) -> None
    |
    |      Initializes a zero-dimensional box.
    |
    |      2. __init__(self: _cQuadTree.Extent, left: float, bottom: float, width: float, height: float) -> None
    |
    |      Initialize with position of bottom left corner, as well as width and height.
    |
    |  __repr__(...)
    |      __repr__(self: _cQuadTree.Extent) -> str
    |
    |      Get string representation of object
    |
    |  b(...)
    |      b(self: _cQuadTree.Extent) -> float
    |
    |  bottom(...)
    |      bottom(self: _cQuadTree.Extent) -> float
    |
    |  h(...)
    |      h(self: _cQuadTree.Extent) -> float
    |
    |  height(...)
    |      height(self: _cQuadTree.Extent) -> float
    |
    |  l(...)
    |      l(self: _cQuadTree.Extent) -> float
    |
    |  left(...)
    |      left(self: _cQuadTree.Extent) -> float
    |
    |  w(...)
    |      w(self: _cQuadTree.Extent) -> float
    |
    |  width(...)
    |      width(self: _cQuadTree.Extent) -> float

Point
^^^^^

.. code:: python

   class Point(pybind11_builtins.pybind11_object)
    |  Minimal 2D-vector implementation based on code by openFrameworks
    |
    |  Method resolution order:
    |      Point
    |      pybind11_builtins.pybind11_object
    |      builtins.object
    |
    |  Methods defined here:
    |
    |  __init__(...)
    |      __init__(*args, **kwargs)
    |      Overloaded function.
    |
    |      1. __init__(self: _cQuadTree.Point) -> None
    |
    |      2. __init__(self: _cQuadTree.Point, x: float, y: float) -> None
    |
    |      Initialize with coordinates
    |
    |  __repr__(...)
    |      __repr__(self: _cQuadTree.Point) -> str
    |
    |      Get string representation of object
    |
    |  length(...)
    |      length(self: _cQuadTree.Point) -> float
    |
    |      Get the length of the vector
    |
    |  ----------------------------------------------------------------------
    |  Data descriptors defined here:
    |
    |  x
    |
    |  y

Histogram
^^^^^^^^^

.. code:: python

   >>> from cQuadTree import histogram
   >>> help(histogram)

   histogram(data, counts, bin_edges, density=True)
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

Get points and boxes
^^^^^^^^^^^^^^^^^^^^

.. code:: python

   >>> from cQuadTree.utils import get_points_and_boxes
   >>> help(get_points_and_boxes)

   get_points_and_boxes(quadtree)
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

Plot boxes
~~~~~~~~~~

.. code:: python

   >>> from cQuadTree.plot import plot_box_tree
   >>> help(plot_box_tree)

   plot_box_tree(list_of_points, list_of_boxes, ax=None, box_kwargs={}, point_kwargs={})
       Plot a graphical representation of the tree as boxes and points on a matplotlib.Axes.

       Use with data obtained from :func:`cQuadTree.utils.get_points_and_boxes`.

Changelog
---------

Changes are logged in a `separate
file <https://github.com/benmaier/cQuadTree/blob/main/CHANGELOG.md>`__.

License
-------

This project is licensed under the `MIT
License <https://github.com/benmaier/cQuadTree/blob/main/LICENSE>`__.
Note that this excludes any images/pictures/figures shown here or in the
documentation.

Contributing
------------

If you want to contribute to this project, please make sure to read the
`code of
conduct <https://github.com/benmaier/cQuadTree/blob/main/CODE_OF_CONDUCT.md>`__
and the `contributing
guidelines <https://github.com/benmaier/cQuadTree/blob/main/CONTRIBUTING.md>`__.
In case you're wondering about what to contribute, we're always
collecting ideas of what we want to implement next in the `outlook
notes <https://github.com/benmaier/cQuadTree/blob/main/OUTLOOK.md>`__.

|Contributor Covenant|

Dev notes
---------

Fork this repository, clone it, and install it in dev mode.

.. code:: bash

   git clone git@github.com:YOURUSERNAME/cQuadTree.git
   make

If you want to upload to PyPI, first convert the new ``README.md`` to
``README.rst``

.. code:: bash

   make readme

It will give you warnings about bad ``.rst``-syntax. Fix those errors in
``README.rst``. Then wrap the whole thing

.. code:: bash

   make pypi

It will probably give you more warnings about ``.rst``-syntax. Fix those
until the warnings disappear. Then do

.. code:: bash

   make upload

.. |Contributor Covenant| image:: https://img.shields.io/badge/Contributor%20Covenant-v1.4%20adopted-ff69b4.svg
   :target: code-of-conduct.md
