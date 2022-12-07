# cQuadTree

Infinite jest with your favorite space-dividing trees. Fork from this C++-codebase: benmaier/BarnesHutTree

* repository: https://github.com/benmaier/cQuadTree/
* documentation: http://cQuadTree.benmaier.org/

```python 
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
```

## Install

Your machine needs to have a C++11-compatible compiler installed.

    pip install cQuadTree

So far, the package's functionality was tested on Mac OS X and CentOS only.

## Dependencies

`cQuadTree` directly depends on the following packages which will be installed by `pip` during the installation process

* `numpy>=1.20`

## Documentation

### Build the tree

```python
import numpy as np
from cQuadTree import QuadTree
points = np.random.rand(100,2).tolist()
T = QuadTree(points)
```

### Explore the tree recursively

As an example, here's a recursive function that collects all internal node boxes and leaf's points

```python
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
```

### Compute the force on a point

```python
>>> T.compute_force(point=(0.,0.001),theta=1.0) # default is theta=0.5
(0.117681690892212, 0.20856460584929215)
```

### Get all distances to a point

Note that per default, distances of value zero will be disregarded.
If you want to include those, set `ignore_zero_distance=False` in the
function call.

```python
>>> T.get_distances_to((0.,0),theta=1) # default is theta=0.2
[(8.705170877128145, 2), (6.040695324215583, 1), (2.23606797749979, 1)]
```

The first number is the distance to the query point, the second is the
number of points that lie at this approximate distance to the query point.

### Get all distances between pairs of points in a list and points in the tree

```python
>>> T.get_distances_to_points(points,theta=1)
[(6.7364679172397155, 2), (5.166236541235796, 1), (2.630589287593181, 1), (11.013627921806693, 1), (8.050465825031493, 1), (2.630589287593181, 1), (8.668333173107735, 1), (5.4230987451825, 1), (9.822932352408825, 2), (5.166236541235796, 1)]
```

### Get all pairwise distances between points in the tree

```python
>>> T.get_pairwise_distances(theta=1.0)
[(2.630589287593181, 1), (11.013627921806693, 1), (8.050465825031493, 1), (2.630589287593181, 1), (8.668333173107735, 1), (5.4230987451825, 1), (9.822932352408825, 2), (5.166236541235796, 1), (6.7364679172397155, 2), (5.166236541235796, 1)]
```

### Build a distance histogram from distance counts

```python
from cQuadTree import histogram
dists, counts = zip(*T.get_pairwise_distances(theta=1.0))
bin_edges = np.logspace(-4,1/2,101,base=2)
pdf, _ = histogram(dists, counts, bin_edges)
```

### Plot tree as boxes and points

```python
from cQuadTree import get_points_and_boxes
from cQuadTree.plot import plot_box_tree

plot_box_tree(*get_points_and_boxes(T))
```

![Box representation of tree](https://github.com/benmaier/cQuadTree/blob/main/img/boxtree.png?raw=true)

## Changelog

Changes are logged in a [separate file](https://github.com/benmaier/cQuadTree/blob/main/CHANGELOG.md).

## License

This project is licensed under the [MIT License](https://github.com/benmaier/cQuadTree/blob/main/LICENSE).
Note that this excludes any images/pictures/figures shown here or in the documentation.

## Contributing

If you want to contribute to this project, please make sure to read the [code of conduct](https://github.com/benmaier/cQuadTree/blob/main/CODE_OF_CONDUCT.md) and the [contributing guidelines](https://github.com/benmaier/cQuadTree/blob/main/CONTRIBUTING.md). In case you're wondering about what to contribute, we're always collecting ideas of what we want to implement next in the [outlook notes](https://github.com/benmaier/cQuadTree/blob/main/OUTLOOK.md).

[![Contributor Covenant](https://img.shields.io/badge/Contributor%20Covenant-v1.4%20adopted-ff69b4.svg)](code-of-conduct.md)

## Dev notes

Fork this repository, clone it, and install it in dev mode.

```bash
git clone git@github.com:YOURUSERNAME/cQuadTree.git
make
```

If you want to upload to PyPI, first convert the new `README.md` to `README.rst`

```bash
make readme
```

It will give you warnings about bad `.rst`-syntax. Fix those errors in `README.rst`. Then wrap the whole thing 

```bash
make pypi
```

It will probably give you more warnings about `.rst`-syntax. Fix those until the warnings disappear. Then do

```bash
make upload
```
