// geo - a fast 2D geometry library for ruby
// Copyright (C) 2007  Martin Kihlgren <zond at troja dot ath dot cx>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.


#ifndef TYPES_H
#define TYPES_H

typedef struct {
  gdouble x;
  gdouble y;
  VALUE rbPoint;
} Point;

typedef struct {
  Point *p1;
  Point *p2;
  VALUE rbLine;
} Line;

typedef enum {
  itPOINT,
  itLINE,
} IntersectionType;

typedef struct {
  Point *point;
  Line *line;
  Line *with;
  IntersectionType type;
} Intersection;

typedef struct {
  Point *p1;
  Point *p2;
  Point *p3;
  VALUE rbTriangle;
} Triangle;

typedef struct {
  GHashTable *table; // key and value are the content of this GeoSet
  gboolean iterating; // TRUE if iterating right now
  Point *bottom_left;
  Point *top_right;
  gdouble segment_side;
  GHashTable *segments; // x-index_from_left(0) << 16 + y-index_from_top(ymax)
  GHashTable *segment_lines; // x-index_from_left(0) << 16 + y-index_from_top(ymax)
  gboolean index_dirty;
  gboolean indexed;
  VALUE rbGeoSet;
} GeoSet;


#endif
