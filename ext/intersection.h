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


#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <common.h>


#define INTERSECTION_CLOSEST_POINT(intersection_pointer,point_pointer) ((intersection_pointer)->type == itPOINT ? \
									(intersection_pointer)->point : \
									(DISTANCE((intersection_pointer)->line->p1, \
										  (point_pointer)) < DISTANCE((intersection_pointer)->line->p2, \
													      (point_pointer)) ? \
									 (intersection_pointer)->line->p1 : \
									 (intersection_pointer)->line->p2))
#define INTERSECTION_DISTANCE(intersection_pointer,point_pointer) ((intersection_pointer)->type == itPOINT ? \
								   DISTANCE((intersection_pointer)->point, (point_pointer)) : \
								   MIN(DISTANCE((intersection_pointer)->line->p1, (point_pointer)), \
								       DISTANCE((intersection_pointer)->line->p2, (point_pointer))))


void
free_intersection(Intersection *intersection);

Intersection*
new_intersection_with_line(Line *l, Line *with);

Intersection *
new_intersection_with_point(Point *p, Line *with);

VALUE
rb_intersection(Intersection *intersection, Point *from);






#endif
