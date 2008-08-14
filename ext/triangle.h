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


#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <common.h>


extern VALUE rb_triangle;

#define RB_TRIANGLE(triangle_pointer,klass) ((triangle_pointer)->rbTriangle = Data_Wrap_Struct(klass, triangle_mark, free, (triangle_pointer)))
#define TRIANGLE(rb_triangle,triangle_pointer) Data_Get_Struct((rb_triangle), Triangle, (triangle_pointer))
#define TRIANGLE_EQUALS(t1,t2) (POINT_EQUALS((t1)->p1, (t2)->p1) && POINT_EQUALS((t1)->p2, (t2)->p2) && POINT_EQUALS((t1)->p3, (t2)->p3))
#define TRIANGLE_P(t) (!NIL_P((t)) && rb_is_a((t), rb_triangle))
#define CHECK_TRIANGLE(t) if (!TRIANGLE_P((t))) rb_raise(rb_eTypeError, "Expected Geo::Triangle!")
#define TRIANGLE_BOUNDS(t,bottom_left,top_right) (bottom_left)->x = MIN(t->p1->x, MIN(t->p2->x, t->p3->x)); \
  (bottom_left)->y = MIN(t->p1->y, MIN(t->p2->y, t->p3->y));		\
  (top_right)->x = MAX(t->p1->x, MAX(t->p2->x, t->p3->x));		\
  (top_right)->y = MAX(t->p1->y, MAX(t->p2->y, t->p3->y))

VALUE
rb_triangle_alloc(VALUE class);

VALUE
rb_triangle_initialize(int argc, VALUE *argv, VALUE self);

VALUE
rb_triangle_inspect(VALUE self);

VALUE
rb_triangle_set_p1(VALUE self, VALUE new_point);

VALUE
rb_triangle_set_p2(VALUE self, VALUE new_point);

VALUE
rb_triangle_set_p3(VALUE self, VALUE new_point);

VALUE
rb_triangle_p1(VALUE self);

VALUE
rb_triangle_p2(VALUE self);

VALUE
rb_triangle_p3(VALUE self);

VALUE
rb_triangle_overlap(VALUE self, VALUE line);

VALUE
rb_triangle_equals(VALUE self, VALUE o);

VALUE
rb_triangle_cmp(VALUE self, VALUE o);

VALUE
rb_triangle_clone(VALUE self);

VALUE
rb_triangle_area(VALUE self);

gboolean
triangle_contains(Triangle *triangle, Point *point);

VALUE
rb_triangle_contains(VALUE self, VALUE p);

gboolean
triangle_intersects(Triangle *triangle, Line *line);

VALUE
rb_triangle_intersects(VALUE self, VALUE line);

VALUE
rb_triangle_from_gpointer(gpointer l);

gchar*
triangle_inspect(Triangle *t);

Line*
triangle_overlap(Triangle *triangle, Line *line);





#endif
