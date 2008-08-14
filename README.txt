= georuby_c

Git clone of http://geo.rubyforge.org. A 2D geometry library for ruby.

== DESCRIPTION:

geo is a ruby gem that provides a fast 2D geometry library.
It provides fast operations for points, lines, triangles, intersections between lines 
and triangles and triangles containing points.

== REQUIREMENTS:

glib:: http://www.gtk.org/

== FEATURES:

Geo::Point:: A 2D point providing some common geometry operations.
Geo::Line:: A 2D line consisting of 2 Geo::Points providing some common geometry operations.
Geo::Triangle:: A 2D triangle consisting of 3 Geo::Points providing some common geometry operations.
Geo::PointSet:: A Set-like container of Points.
Geo::LineSet:: A Set-like container of Lines that provides optimized versions of some common geometry operations on lines.
Geo::TriangleSet:: A Set-like container of Triangles that provides optimized versions of some common geometry operations on lines.

== Usage:

To use geo just install the gem.

== Examples:

To find if a given line intersects a set of 100 000 other lines:

 :include:examples/intersects.rb

== License:

This library is provided under the GPL-2.