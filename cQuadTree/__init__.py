# -*- coding: utf-8 -*-
"""
Initializes this package with metadata.
"""

from .metadata import (
        __version__,
        __author__,
        __copyright__,
        __credits__,
        __license__,
        __maintainer__,
        __email__,
        __status__,
    )

from _cQuadTree import (
        Point,
        Extent,
        QuadTree,
    )

from .utils import (
        histogram,
        get_points_and_boxes,
    )
