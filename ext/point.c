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

VALUE rb_point;

const Point ORIGO = { 0.0, 0.0, 0 };

Point*
new_point(gdouble x, gdouble y) {
  Point *rval = ALLOC(Point);
  rval->x = x;
  rval->y = y;
  rval->rbPoint = 0;
  return rval;
}


VALUE
rb_point_alloc(VALUE class) {
  Point *p = ALLOC(Point);
  return RB_POINT(p, class);
}

VALUE
rb_point_initialize(VALUE self, VALUE x, VALUE y) {
  Point *p;
  CHECK_NUMERICALITY(x);
  CHECK_NUMERICALITY(y);
  POINT(self, p);
  p->x = NUM2DBL(x);
  p->y = NUM2DBL(y);
  return self;
}

VALUE
rb_point_set_x(VALUE self, VALUE new_x) {
  Point *p;
  CHECK_NUMERICALITY(new_x);
  POINT(self, p);
  p->x = NUM2DBL(new_x);
  return new_x;
}

VALUE
rb_point_set_y(VALUE self, VALUE new_y) {
  Point *p;
  CHECK_NUMERICALITY(new_y);
  POINT(self, p);
  p->y = NUM2DBL(new_y);
  return new_y;
}

VALUE
rb_point_x(VALUE self) {
  Point *p;
  POINT(self, p);
  return rb_float_new(p->x);
}

VALUE
rb_point_y(VALUE self) {
  Point *p;
  POINT(self, p);
  return rb_float_new(p->y);
}

gchar*
point_inspect(Point *p) {
  gchar *rval = calloc(1024, sizeof(gchar));
  snprintf(rval, 1024, "<%s:%p x=%f y=%f>", rb_obj_classname(p->rbPoint), p, p->x, p->y);
  return rval;
}

VALUE
rb_point_inspect(VALUE self) {
  gchar *str;
  Point *p;
  VALUE rval;
  POINT(self, p);
  str = point_inspect(p);
  rval = rb_str_new2(str);
  free(str);
  return rval;
}


VALUE
rb_point_dot(VALUE self, VALUE o) {
  Point *me;
  Point *point;
  CHECK_POINT(o);
  POINT(self, me);
  POINT(o, point);
  return rb_float_new(POINT_DOT(me, point));
}

VALUE
rb_point_abs(VALUE self) {
  Point *me;
  POINT(self, me);
  return rb_float_new(DISTANCE(&ORIGO, me));
}

gboolean
point_equals(gconstpointer p1, gconstpointer p2) {
  return POINT_EQUALS(p1, p2);
}

VALUE
rb_point_equals(VALUE self, VALUE o) {
  if (!POINT_P(o)) {
    return Qfalse;
  } else {
    Point *me;
    Point *other;
    POINT(self, me);
    POINT(o, other);
    if (POINT_EQUALS(me, other)) {
      return Qtrue;
    } else {
      return Qfalse;
    }
  }
}

gint
point_cmp(Point *me, Point *other) {
  gdouble my_length;
  gdouble other_length;
  my_length = LENGTH(me->x, me->y);
  other_length = LENGTH(other->x, other->y);
  if (my_length != other_length)
    return CMP(my_length, other_length);
  else if (me->x != other->x)
    return CMP(me->x, other->x);
  else
    return CMP(me->y, other->y);
}

VALUE
rb_point_cmp(VALUE self, VALUE o) {
  if (!POINT_P(o)) {
    return INT2NUM(CMP(CLASS(self), CLASS(o)));
  } else {
    Point *me;
    Point *other;
    POINT(self, me);
    POINT(o, other);
    return INT2NUM(point_cmp(me, other));
  }
}

VALUE
rb_point_set_angle(VALUE self, VALUE new_angle) {
  Point *me;
  gdouble length;
  gdouble angle;
  CHECK_NUMERICALITY(new_angle);
  POINT(self, me);
  angle = NUM2DBL(new_angle);
  length = LENGTH(me->x, me->y);
  while (angle > M_PI * 2)
    angle -= M_PI * 2;
  me->x = length * cos(angle);
  me->y = length * sin(angle);
  return rb_float_new(angle);
}

VALUE
rb_point_angle(VALUE self) {
  Point *me;
  POINT(self, me);
  ZERO_LENGTH_CHECK(me);
  return rb_float_new(ANGLE(me->x, me->y));
}

VALUE
rb_point_set_abs(VALUE self, VALUE new_abs) {
  Point *me;
  gdouble ratio;
  CHECK_NUMERICALITY(new_abs);
  POINT(self, me);
  ZERO_LENGTH_CHECK(me);
  ratio = NUM2DBL(new_abs) / LENGTH(me->x, me->y);
  me->x = me->x * ratio;
  me->y = me->y * ratio;
  return new_abs;
}

VALUE
rb_point_clone(VALUE self) {
  Point *me;
  Point *rval;
  POINT(self, me);
  rval = new_point(me->x, me->y);
  return RB_POINT(rval, CLASS(self));
}

VALUE
rb_point_to(int argc, VALUE *argv, VALUE self) {
  Point *me;
  Point *other_end;
  Line *rval;
  if (argc == 1) {
    CHECK_POINT(argv[0]);
    POINT(self, me);
    POINT(argv[0], other_end);
    rval = new_line_with_points(me, other_end);
    return RB_LINE(rval, rb_line);
  } else if (argc == 2) {
    CHECK_NUMERICALITY(argv[0]);
    CHECK_NUMERICALITY(argv[1]);
    POINT(self, me);
    other_end = new_point(NUM2DBL(argv[0]), NUM2DBL(argv[1]));
    RB_POINT(other_end, CLASS(self));
    rval = new_line_with_points(me, other_end);
    return RB_LINE(rval, rb_line);
  } else {
    rb_raise(rb_eTypeError, "Arguments to %s#to has to be either a Geo::Point or 2 numbers.", rb_obj_classname(self));
  }
}

VALUE
rb_point_on(VALUE self, VALUE l) {
  Point *me;
  Line *line;
  gint side;
  CHECK_LINE(l);
  POINT(self,me);
  LINE(l, line);
  side = line_side(line, me);
  if (side == 0) {
    return Qtrue;
  } else {
    return Qfalse;
  }
}

VALUE
rb_point_in(VALUE self, VALUE t) {
  Point *me;
  Triangle *triangle;
  CHECK_TRIANGLE(t);
  POINT(self, me);
  TRIANGLE(t, triangle);
  if (triangle_contains(triangle, me)) {
    return Qtrue;
  } else {
    return Qfalse;
  }
}

VALUE
rb_point_plus(VALUE self, VALUE p) {
  Point *me;
  Point *other;
  Point *rval;
  CHECK_POINT(p);
  POINT(self, me);
  POINT(p, other);
  rval = ALLOC(Point);
  rval->x = me->x + other->x;
  rval->y = me->y + other->y;
  return RB_POINT(rval, CLASS(self));
}

VALUE
rb_point_minus(VALUE self, VALUE p) {
  Point *me;
  Point *other;
  Point *rval;
  CHECK_POINT(p);
  POINT(self, me);
  POINT(p, other);
  rval = ALLOC(Point);
  rval->x = me->x - other->x;
  rval->y = me->y - other->y;
  return RB_POINT(rval, CLASS(self));
}

guint
point_hash(gconstpointer p) {
  Point *point = (Point *) p;
  return (guint) (point->x + point->y);
}

