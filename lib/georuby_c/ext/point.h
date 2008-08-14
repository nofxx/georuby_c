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


#ifndef POINT_H
#define POINT_H

#include <common.h>

extern VALUE rb_point;

#define RB_POINT(point_pointer,klass) ((point_pointer)->rbPoint = Data_Wrap_Struct(klass, NULL, free, (point_pointer)))
#define POINT(rb_point,point_pointer) Data_Get_Struct((rb_point), Point, (point_pointer))

#define LENGTH(dx,dy) (sqrt(pow((dx),2) + pow((dy),2)))
#define DISTANCE(p1,p2) (LENGTH((p1)->x - (p2)->x, (p1)->y - (p2)->y))
#define ANGLE(dx,dy) ((dy) > 0 ?					\
		      ((dx) > 0 ?					\
		       acos((dx) / LENGTH((dx),(dy)))			\
		       :						\
		       ((dx) < 0 ?					\
			acos((dx) / LENGTH((dx),(dy)))			\
			:						\
			M_PI_2))					\
		      :							\
		      ((dy) < 0 ?					\
		       ((dx) > 0 ?					\
			(2 * M_PI - acos((dx) / LENGTH((dx),(dy))))	\
			:						\
			((dx) < 0 ?					\
			 (2 * M_PI - acos((dx) / LENGTH((dx),(dy))))	\
			 :						\
			 M_PI_2 + M_PI))				\
		       :						\
		       ((dx) > 0 ?					\
			0.0						\
			:						\
			M_PI)))

#define POINT_EQUALS(p1,p2) (DBL_EQL(((Point *) (p1))->x, ((Point *) (p2))->x) && DBL_EQL(((Point *) (p1))->y, ((Point *) (p2))->y))
#define POINT_DOT(p1,p2) ((p1)->x * (p2)->x + (p1)->y * (p2)->y)
#define POINT_P(p) (!NIL_P((p)) && rb_is_a((p), rb_point))
#define CHECK_POINT(p) if (!POINT_P((p))) rb_raise(rb_eTypeError, "Expected Geo::Point!")
#define ZERO_LENGTH_P(p) ((p)->x == 0 && (p)->y == 0)
#define ZERO_LENGTH_CHECK(p) if (ZERO_LENGTH_P((p))) rb_raise(rb_eTypeError, "Expected something with length > 0!")
#define ZERO_DISTANCE_P(p1,p2) ((p1)->x == (p2)->x && ((p1)->y == (p2)->y))
#define ZERO_DISTANCE_CHECK(p1,p2) if (ZERO_DISTANCE_P((p1),(p2))) rb_raise(rb_eTypeError, "Expected something with distance > 0!")

extern const Point ORIGO;


Point*
new_point(gdouble x, gdouble y);

VALUE
rb_point_alloc(VALUE class);

VALUE
rb_point_initialize(VALUE self, VALUE x, VALUE y);

VALUE
rb_point_set_x(VALUE self, VALUE new_x);

VALUE
rb_point_set_y(VALUE self, VALUE new_y);

VALUE
rb_point_x(VALUE self);

VALUE
rb_point_y(VALUE self);

VALUE
rb_point_inspect(VALUE self);

VALUE
rb_point_dot(VALUE self, VALUE o);

VALUE
rb_point_abs(VALUE self);

VALUE
rb_point_equals(VALUE self, VALUE o);

VALUE
rb_point_cmp(VALUE self, VALUE o);

VALUE
rb_point_set_angle(VALUE self, VALUE new_angle);

VALUE
rb_point_angle(VALUE self);

VALUE
rb_point_set_abs(VALUE self, VALUE new_abs);

VALUE
rb_point_clone(VALUE self);

VALUE
rb_point_to(int argc, VALUE *argv, VALUE self);

VALUE
rb_point_on(VALUE self, VALUE l);

VALUE
rb_point_in(VALUE self, VALUE t);

VALUE
rb_point_plus(VALUE self, VALUE p);

VALUE
rb_point_minus(VALUE self, VALUE p);

gint
point_cmp(Point *me, Point *other);

gchar*
point_inspect(Point *p);

guint
point_hash(gconstpointer p);

gboolean
point_equals(gconstpointer a, gconstpointer b);



#endif

