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


#include <common.h>


void
free_intersection(Intersection *intersection) {
  if (intersection->type == itPOINT)
    free(intersection->point);
  else if (intersection->type == itLINE)
    line_free(intersection->line);
  free(intersection);
}

Intersection*
new_intersection_with_line(Line *l, Line *with) {
  Intersection *rval = ALLOC(Intersection);
  rval->point = NULL;
  rval->line = l;
  rval->with = with;
  rval->type = itLINE;
  return rval;
}

Intersection *
new_intersection_with_point(Point *p, Line *with) {
  Intersection *rval = ALLOC(Intersection);
  rval->point = p;
  rval->line = NULL;
  rval->with = with;
  rval->type = itPOINT;
  return rval;
}

VALUE
rb_intersection(Intersection *intersection, Point *from) {
  if (intersection == NULL) {
    return Qnil;
  } else {
    VALUE rval;
    if (intersection->type == itLINE) {
      if (DISTANCE(from, intersection->line->p1) < DISTANCE(from, intersection->line->p2)) {
	Point *point = new_point(intersection->line->p1->x, intersection->line->p1->y);
	rval = RB_POINT(point, rb_point);
      } else {
	Point *point = new_point(intersection->line->p2->x, intersection->line->p2->y);
	rval = RB_POINT(point, rb_point);
      }
    } else if (intersection->type == itPOINT) {
      Point *point = new_point(intersection->point->x, intersection->point->y);
      rval = RB_POINT(point, rb_point);
    } else {
      rb_raise(rb_eRuntimeError, "Weird, an intersection should contain either a point or a line!");
    }
    free_intersection(intersection);
    return rval;
  }
}

