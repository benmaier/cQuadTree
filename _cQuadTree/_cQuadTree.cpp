/*
 * The MIT License (MIT)
 * Copyright (c) 2022, Benjamin F. Maier
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall
 * be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-
 * INFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 * OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>
#include <tuple>
#include <Point.h>
#include <QuadTree.h>

using namespace std;
namespace py = pybind11;

PYBIND11_MODULE(_cQuadTree, m)
{
    m.doc() = R"pbdoc(
        C++-core of cQuadTree.

        .. currentmodule:: _cQuadTree

        Classes
        -------

        .. autosummary::
            :toctree: _generate

            QuadTree
            Extent
            Point

    )pbdoc";

    py::class_<Point>(m, "Point", R"pbdoc(Minimal 2D-vector implementation based on code by openFrameworks)pbdoc")
        .def(py::init<>())
        .def(py::init<const double &, const double &>(),
             py::arg("x"),
             py::arg("y"),
             "Initialize with coordinates")
        .def("length", &Point::length, R"pbdoc(Get the length of the vector)pbdoc")
        .def("__repr__", &Point::tostr, R"pbdoc(Get string representation of object)pbdoc")
        .def_readwrite("x", &Point::x)
        .def_readwrite("y", &Point::y)
    ;

    py::class_<Extent>(m, "Extent", R"pbdoc(A rectangular geometry.)pbdoc")
        .def(py::init<>(),"Initializes a zero-dimensional box.")
        .def(py::init<const double &, const double &, const double &, const double &>(),
             py::arg("left"),
             py::arg("bottom"),
             py::arg("width"),
             py::arg("height"),
             "Initialize with position of bottom left corner, as well as width and height.")
        .def("__repr__", &Extent::tostr, R"pbdoc(Get string representation of object)pbdoc")
        .def("left", &Extent::left)
        .def("bottom", &Extent::bottom)
        .def("width", &Extent::width)
        .def("height", &Extent::height)
        .def("l", &Extent::left)
        .def("b", &Extent::bottom)
        .def("w", &Extent::width)
        .def("h", &Extent::height)
    ;

    py::class_<QuadTree>(m, "QuadTree", R"pbdoc(A QuadTree.)pbdoc")
        .def(py::init<>(),"Initialize an empty tree.")
        .def(py::init< vector < pair < double, double > > &,
                       bool const &
                     >(),
             py::arg("position_pairs"/*, "List of 2-Tuples containing (x, y)-positions"*/),
             py::arg("force_square"/*, "Whether or not to force the tree into a square geometry")*/) = true,
             "Initialize a tree given a list of positions.")
        .def(py::init< vector < pair < double, double > > &,
                       vector < double > &,
                       bool const &
                     >(),
             py::arg("position_pairs"/*, "List of 2-Tuples containing (x, y)-positions"*/),
             py::arg("masses"/*, "List of masses corresponding to the positions"*/),
             py::arg("force_square"/*, "Whether or not to force the tree into a square geometry")*/) = true,
             "Initialize a tree given a list of positions and a list of corresponding masses.")
        .def("__repr__", &QuadTree::tostr, R"pbdoc(Get string representation of object)pbdoc")
        .def("__str__", &QuadTree::str, R"pbdoc(Get a string representation of the full tree)pbdoc")
        .def("get_subtrees", &QuadTree::get_subtrees, R"pbdoc(Get a list of all of this node's children that contain data.)pbdoc",py::return_value_policy::reference)
        .def("get_subtree", &QuadTree::get_subtree, R"pbdoc(Get subtree 0<=i<=3.)pbdoc",py::return_value_policy::reference)
        .def("compute_force", &QuadTree::compute_force_on_pair,
                py::arg("point"),
                py::arg("theta")=0.5,
            R"pbdoc(
            Compute the force on a single point using the Barnes-Hut-Algorithm
            with cutoff parameter :math:`\theta`.

            Parameters
            ----------
            point : 2-tuple of float
                Point in plane on which to compute the total force
            theta : float, default = 0.5
                If the distance between the point and the current internal node's
                center of mass is smaller than :math:`\theta` times the diameter
                of the internal node's extent (box), the algorithm will treat
                all children of this node as a giant point mass located at the
                center of mass of this internal node.

            Returns
            -------
            force : 2-tuple of float
                Evaluated force vector
        )pbdoc")
        .def("get_distances_to", &QuadTree::get_distances_to_pair,
                py::arg("point"),
                py::arg("theta") = 0.2,
                py::arg("ignore_zero_distance") = true,
                py::arg("tree") = (QuadTree*) nullptr,
            R"pbdoc(
            Compute distances of point masses and mass clusters to a single point 
            using the Barnes-Hut-Algorithm with cutoff parameter :math:`\theta`.

            Parameters
            ----------
            point : 2-tuple of float
                Points in the plane to which to measure the distances
            theta : float, default = 0.2
                If the distance between the point and the current internal node's
                center of mass is smaller than :math:`\theta` times the diameter
                of the internal node's extent (box), the algorithm will treat
                all children of this node as a giant point mass located at the
                center of mass of this internal node.
            ignore_zero_distance : bool, default = True
                If the distance is zero, do or do not include this result in 
                the ``distance_counts``-list.

            Returns
            -------
            distance_counts : list of 2-tuple of double, int
                An item of this list is a distance-count pair,
                the first entry of the tuple containing a distance
                to the query point and the second entry being the
                number of points that lie at that approximate distance
                to the query point, such that it will look like this

                .. code:: python

                    [
                        (0.2, 1),
                        (0.1, 1),
                        (1.5, 32),
                        ...
                    ]
        )pbdoc")
        .def("get_distances_to_points", &QuadTree::get_distances_to_pairs,
                py::arg("points"),
                py::arg("theta") = 0.2,
                py::arg("ignore_zero_distance") = true,
                py::arg("tree") = (QuadTree*) nullptr,
            R"pbdoc(
            Compute distances of point masses and mass clusters to a list of points
            using the Barnes-Hut-Algorithm with cutoff parameter :math:`\theta`.

            Parameters
            ----------
            points : 2-tuple of float
                List of points in the plane to which to measure the distances
            theta : float, default = 0.2
                If the distance between the point and the current internal node's
                center of mass is smaller than :math:`\theta` times the diameter
                of the internal node's extent (box), the algorithm will treat
                all children of this node as a giant point mass located at the
                center of mass of this internal node.
            ignore_zero_distance : bool, default = True
                If the distance is zero, do or do not include this result in 
                the ``distance_counts``-list.

            Returns
            -------
            distance_counts : list of 2-tuple of double, int
                An item of this list is a distance-count pair,
                the first entry of the tuple containing a distance
                to the query point and the second entry being the
                number of points that lie at that approximate distance
                to the query point, such that it will look like this

                .. code:: python

                    [
                        (0.2, 1),
                        (0.1, 1),
                        (1.5, 32),
                        ...
                    ]
        )pbdoc")
        .def("get_pairwise_distances", &QuadTree::_get_pairwise_distances,
                py::arg("theta") = 0.2,
                py::arg("ignore_zero_distance") = true,
            R"pbdoc(
            Compute distances between pairs of points and point clusters 
            of a tree using the Barnes-Hut-Algorithm with cutoff parameter
            :math:`\theta`.

            Iterates over points by querying the tree recursively, which 
            might take longer than simply externally iterating over a list of points
            if they're known.

            Parameters
            ----------
            theta : float, default = 0.2
                If the distance between the point and the current internal node's
                center of mass is smaller than :math:`\theta` times the diameter
                of the internal node's extent (box), the algorithm will treat
                all children of this node as a giant point mass located at the
                center of mass of this internal node.
            ignore_zero_distance : bool, default = True
                If the distance is zero, do or do not include this result in 
                the ``distance_counts``-list.

            Returns
            -------
            distance_counts : list of 2-tuple of double, int
                An item of this list is a distance-count pair,
                the first entry of the tuple containing a distance
                to the query point and the second entry being the
                number of points that lie at that approximate distance
                to the query point, such that it will look like this

                .. code:: python

                    [
                        (0.2, 1),
                        (0.1, 1),
                        (1.5, 32),
                        ...
                    ]
        )pbdoc")
        .def("is_leaf", &QuadTree::is_leaf, "Whether or not this node is a leaf.")



        .def_readwrite("geom", &QuadTree::geom, "Extent of box this tree represents.")
        .def_readwrite("current_data_quadrant", &QuadTree::current_data_quadrant, "Quadrant of the parent geometry the data of this tree resides in.")


        .def_readwrite("geom", &QuadTree::geom, "Extent of box this tree represents.")
        .def_readwrite("current_data_quadrant", &QuadTree::current_data_quadrant, "Quadrant of the parent geometry the data of this tree resides in.")
        .def_readwrite("this_pos", &QuadTree::this_pos, "Position of the point contained in this leaf.")
        .def_readwrite("this_id", &QuadTree::this_id, "Data index of the point contained in this leaf.")
        .def_readwrite("this_mass", &QuadTree::this_mass, "Mass the point contained in this leaf.")
        .def_readwrite("total_mass", &QuadTree::total_mass, "Total mass of all points contained in this internal node.")
        .def_readwrite("total_mass_position", &QuadTree::total_mass_position, "Sum of product of mass and position of all points contained in this internal node.")
        .def_readwrite("center_of_mass", &QuadTree::center_of_mass, "Mass-weighted mean position of all points contained in this internal node.")
        .def_readwrite("number_of_contained_points", 
                  &QuadTree::number_of_contained_points, "Number of points contained in this internal node.")
        .def_readwrite("parent", &QuadTree::parent, "The parent of this internal node.")
    ;

}
