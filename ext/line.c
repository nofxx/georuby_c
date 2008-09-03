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

VALUE rb_line;

/*
 * Dont use this on lines that have been rubified (RB_LINE),
 * since they will be freed by ruby. Make sure that they have
 * rubified points (RB_POINT) as well though!
 */
void
line_free(Line *l) {
  free(l->p1);
  free(l->p2);
  free(l);
}

void
line_mark(Line *line) {
  if (line->p1 != NULL)
    rb_gc_mark(line->p1->rbPoint);
  if (line->p2 != NULL)
    rb_gc_mark(line->p2->rbPoint);
}

Line*
new_line_with_points(Point *p1, Point *p2) {
  Line *rval = ALLOC(Line);
  rval->p1 = p1;
  rval->p2 = p2;
  rval->rbLine = 0;
  return rval;
}

Line*
new_line_with_coordinates(gdouble x1, gdouble y1, gdouble x2, gdouble y2) {
  Line *rval = ALLOC(Line);
  rval->p1 = new_point(x1, y1);
  rval->p2 = new_point(x2, y2);
  rval->rbLine = 0;
  return rval;
}

void
line_destroy_notify(gpointer l) {
  line_free((Line *) l);
}

VALUE
rb_line_alloc(VALUE class) {
  Line *l = ALLOC(Line);
  l->p1 = NULL;
  l->p2 = NULL;
  return RB_LINE(l, class);
}

VALUE
rb_line_initialize(int argc, VALUE *argv, VALUE self) {
  Line *l;
  Point *p1;
  Point *p2;
  if (argc == 2) {
    CHECK_POINT(argv[0]);
    CHECK_POINT(argv[1]);
    POINT(argv[0], p1);
    POINT(argv[1], p2);
  } else if (argc == 4) {
    CHECK_NUMERICALITY(argv[0]);
    CHECK_NUMERICALITY(argv[1]);
    CHECK_NUMERICALITY(argv[2]);
    CHECK_NUMERICALITY(argv[3]);
    p1 = ALLOC(Point);
    p1->x = NUM2DBL(argv[0]);
    p1->y = NUM2DBL(argv[1]);
    p2 = ALLOC(Point);
    p2->x = NUM2DBL(argv[2]);
    p2->y = NUM2DBL(argv[3]);
    RB_POINT(p1, rb_point);
    RB_POINT(p2, rb_point);
  } else {
    rb_raise(rb_eTypeError, "Arguments to %s#new have to be 2 Geo::Points or 4 numbers.", rb_obj_classname(self));
  }
  LINE(self, l);
  l->p1 = p1;
  l->p2 = p2;
  return self;
}

gchar *
line_inspect(Line *l) {
  gchar *rval = calloc(1024, sizeof(gchar));
  gchar *p1 = point_inspect(l->p1);
  gchar *p2 = point_inspect(l->p2);
  snprintf(rval, 1024, "<%s:%p p1=%s p2=%s>",
	   rb_obj_classname(l->rbLine), l, 
	   p1,
	   p2);
  free(p1);
  free(p2);
  return rval;
}

VALUE
rb_line_inspect(VALUE self) {
  gchar *str;
  Line *l;
  VALUE rval;
  LINE(self, l);
  str = line_inspect(l);
  rval = rb_str_new2(str);
  free(str);
  return rval;
}

VALUE
rb_line_set_p1(VALUE self, VALUE new_point) {
  Line *l;
  Point *p;
  CHECK_POINT(new_point);
  LINE(self, l);
  POINT(new_point, p);
  l->p1 = p;
  return new_point;
}

VALUE
rb_line_set_p2(VALUE self, VALUE new_point) {
  Line *l;
  Point *p;
  CHECK_POINT(new_point);
  LINE(self, l);
  POINT(new_point, p);
  l->p2 = p;
  return new_point;
}

VALUE
rb_line_p1(VALUE self) {
  Line *l;
  LINE(self, l);
  return l->p1->rbPoint;
}

VALUE
rb_line_p2(VALUE self) {
  Line *l;
  LINE(self, l);
  return l->p2->rbPoint;
}

/*
 * If argument is a point:
 * Is the point between the endpoints of the line?
 * If argument is a line:
 * Is any of the endpoints of the subject within the endpoints of the object?
 */
VALUE
rb_line_within(VALUE self, VALUE arg) {
  Line *l;
  Point *p;
  Line *other;
  if (CLASS(arg) == rb_point) {
    LINE(self, l);
    POINT(arg, p);
    if (POINT_WITHIN_LINE(l, p)) {
      return Qtrue;
    } else {
      return Qfalse;
    }
  } else if (CLASS(arg) == rb_line) {
    LINE(self, l);
    LINE(arg, other);
    if (LINE_WITHIN_LINE(l, other)) {
      return Qtrue;
    } else {
      return Qfalse;
    }
  } else {
    rb_raise(rb_eTypeError, "Argument to %s#within? has to be either Geo::Line or Geo::Point.", rb_obj_classname(self));
  }
}

/*
 * If argument is a point:
 * Is the point outside the rectangle described by the endpoints of the line?
 * If argument is a line:
 * Is both of the subjects endpoints outside the rectangle described by the endpoints of the object?
 */
VALUE
rb_line_outside(VALUE self, VALUE arg) {
  Line *l;
  Point *p;
  Line *other;
  if (CLASS(arg) == rb_point) {
    LINE(self, l);
    POINT(arg, p);
    if (POINT_OUTSIDE_LINE(l, p)) {
      return Qtrue;
    } else {
      return Qfalse;
    }
  } else if (CLASS(arg) == rb_line) {
    LINE(self, l);
    LINE(arg, other);
    if (LINE_OUTSIDE_LINE(l, other)) {
      return Qtrue;
    } else {
      return Qfalse;
    }
  } else {
    rb_raise(rb_eTypeError, "Argument to %s#outside? has to be either Geo::Line or Geo::Point.", rb_obj_classname(self));
  }
}

VALUE
rb_line_parallel(VALUE self, VALUE p) {
  Line *me;
  Line *rval;
  Point *point;
  CHECK_POINT(p);
  LINE(self, me);
  POINT(p, point);
  rval = LINE_PARALLEL(me, point);
  RB_POINT(rval->p1, CLASS(me->p1->rbPoint));
  RB_POINT(rval->p2, CLASS(me->p2->rbPoint));
  return RB_LINE(rval, CLASS(self));
}

gint
line_side(Line *me, Point *point) {
  if (LINE_VERTICAL(me)) {
    if (DBL_EQL(me->p1->x, point->x)) {
      return 0;
    } else if (DBL_EOG(me->p1->x, point->x)) {
      return 1;
    } else if (DBL_EOL(me->p1->x, point->x)) {
      return -1;
    } else {
      rb_raise(rb_eRuntimeError, "Weird! A point should be either on, left or right of a line!");
    }
  } else {
    gdouble y = LINE_Y(me, point->x);
    if (DBL_EQL(y, point->y)) {
      return 0;
    } else if (DBL_EOG(y, point->y)) {
      return -1;
    } else if (DBL_EOL(y, point->y)) {
      return 1;
    } else {
      rb_raise(rb_eRuntimeError, "Weird! A point should be either on, above or below a line!");
    }
  }
}

/*
 * See http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/
 */
Intersection*
line_intersection(Line *l1, Line *l2) {
  if (LINE_OUTSIDE_LINE(l1, l2)) {
    return NULL;
  } else {
    gdouble x4_x3 = l2->p2->x - l2->p1->x;
    gdouble y1_y3 = l1->p1->y - l2->p1->y;
    gdouble y4_y3 = l2->p2->y - l2->p1->y;
    gdouble x1_x3 = l1->p1->x - l2->p1->x;
    gdouble x2_x1 = l1->p2->x - l1->p1->x;
    gdouble y2_y1 = l1->p2->y - l1->p1->y;
    gdouble denominator = y4_y3 * x2_x1 - x4_x3 * y2_y1;
    if (denominator == 0) { 
      gdouble numerator1 = x4_x3 * y1_y3 - y4_y3 * x1_x3;
      if (numerator1 == 0) {
	gdouble new_x1 = MAX(MIN(l1->p1->x, l1->p2->x), MIN(l2->p1->x, l2->p2->x));
	gdouble new_y1 = MAX(MIN(l1->p1->y, l1->p2->y), MIN(l2->p1->y, l2->p2->y));
	gdouble new_x2 = MIN(MAX(l1->p1->x, l1->p2->x), MAX(l2->p1->x, l2->p2->x));
	gdouble new_y2 = MIN(MAX(l1->p1->y, l1->p2->y), MAX(l2->p1->y, l2->p2->y));
	return new_intersection_with_line(new_line_with_coordinates(new_x1, new_y1, new_x2, new_y2), l1);
      } else {
	return NULL;
      }
    } else {
      gdouble t1 = (x4_x3 * y1_y3 - y4_y3 * x1_x3) / denominator;
      gdouble t2 = (x2_x1 * y1_y3 - y2_y1 * x1_x3) / denominator;
      if (t1 < 0 || t1 > 1 || t2 < 0 || t2 > 1)
	return NULL;
      return new_intersection_with_point(new_point(l1->p1->x + t1 * (l1->p2->x - l1->p1->x),
						   l1->p1->y + t1 * (l1->p2->y - l1->p1->y)), l1);
    }    
  }
}

VALUE
rb_line_intersection(VALUE self, VALUE other_line) {
  Line *me;
  Line *other;
  CHECK_LINE(other_line);
  LINE(self, me);
  LINE(other_line, other);
  return rb_intersection(line_intersection(me, other), other->p1);
}

VALUE
rb_line_dot(VALUE self, VALUE o) {
  Line *me;
  Line *line;
  CHECK_LINE(o);
  LINE(self, me);
  LINE(o, line);
  return rb_float_new(LINE_DOT(line, me));
}

VALUE
rb_line_abs(VALUE self) {
  Line *me;
  LINE(self, me);
  return rb_float_new(DISTANCE(me->p1, me->p2));
}

gboolean
line_equals(gconstpointer l1, gconstpointer l2) {
  return LINE_EQUALS(l1, l2);
}

VALUE
rb_line_equals(VALUE self, VALUE o) {
  if (!LINE_P(o)) {
    return Qfalse;
  } else {
    Line *me;
    Line *other;
    LINE(self, me);
    LINE(o, other);
    if (LINE_EQUALS(me, other)) {
      return Qtrue;
    } else {
      return Qfalse;
    }
  }
}

static gint
line_cmp(Line *me, Line *other) {
  gint tmp;
  gdouble my_length;
  gdouble other_length;
  my_length = DISTANCE(me->p1, me->p2);
  other_length = DISTANCE(other->p1, other->p2);
  if (!DBL_EQL(my_length, other_length))
    return CMP(my_length, other_length);
  else if ((tmp = point_cmp(me->p1, other->p1)) != 0)
    return tmp;
  else
    return point_cmp(me->p2, other->p2);
}

VALUE
rb_line_cmp(VALUE self, VALUE o) {
  if (!LINE_P(o)) {
    return INT2NUM(CMP(CLASS(self), CLASS(o)));
  } else {
    Line *me;
    Line *other;
    LINE(self, me);
    LINE(o, other);
    return INT2NUM(line_cmp(me, other));
  }
}

VALUE
rb_line_reverse(VALUE self) {
  Line *me;
  Line *rval;
  LINE(self, me);
  rval = new_line_with_points(me->p2, me->p1);
  return RB_LINE(rval, CLASS(self));
}

VALUE
rb_line_set_angle(VALUE self, VALUE new_angle) {
  Line *me;
  gdouble length;
  gdouble angle;
  CHECK_NUMERICALITY(new_angle);
  LINE(self, me);
  angle = NUM2DBL(new_angle);
  length = DISTANCE(me->p1, me->p2);
  while (angle > M_PI * 2)
    angle -= M_PI * 2;
  me->p2->x = me->p1->x + length * cos(angle);
  me->p2->y = me->p1->y + length * sin(angle);
  return rb_float_new(angle);
}

VALUE
rb_line_angle(VALUE self) {
  Line *me;
  LINE(self, me);
  ZERO_DISTANCE_CHECK(me->p1, me->p2);
  return rb_float_new(LINE_ANGLE(me));
}

void
line_set_abs(Line *me, double new_abs) {
  double ratio = new_abs / DISTANCE(me->p1, me->p2);
  me->p2->x = me->p1->x + ((me->p2->x - me->p1->x) * ratio);
  me->p2->y = me->p1->y + ((me->p2->y - me->p1->y) * ratio);
}

VALUE
rb_line_set_abs(VALUE self, VALUE new_abs) {
  Line *me;
  CHECK_NUMERICALITY(new_abs);
  LINE(self, me);
  ZERO_DISTANCE_CHECK(me->p1, me->p2);
  line_set_abs(me, NUM2DBL(new_abs));
  return new_abs;
}

VALUE
rb_line_clone(VALUE self) {
  Line *me;
  Line *rval;
  LINE(self, me);
  rval = new_line_with_coordinates(me->p1->x, me->p1->y, me->p2->x, me->p2->y);
  RB_POINT(rval->p1, CLASS(me->p1->rbPoint));
  RB_POINT(rval->p2, CLASS(me->p2->rbPoint));
  return RB_LINE(rval, CLASS(self));
}

static Point*
line_mirror_point(Line *l, Point *p) {
  Point *rval;
  if (DBL_EQL(l->p1->x, l->p2->x)) {
    rval = new_point(-p->x + (2 * l->p1->x),
		     p->y);
  } else {
    gdouble line_k1 = (l->p2->y - l->p1->y) / (l->p2->x - l->p1->x);
    gdouble line_k2 = l->p1->y - (line_k1 * l->p1->x);

    gdouble normal_x_coord = line_k1;
    gdouble normal_y_coord = -1.0;
    gdouble normal_abs = sqrt((normal_x_coord * normal_x_coord) + (normal_y_coord * normal_y_coord));

    // here we translate the point by subtracting line_k2 from p->y to force the projection to be against the line as it crosses origo
    gdouble projection_factor = ((p->x * normal_x_coord + (p->y - line_k2) * normal_y_coord) / 
				 (normal_abs * normal_abs));
    gdouble projection_x_coord = normal_x_coord * projection_factor;
    gdouble projection_y_coord = normal_y_coord * projection_factor;

    rval = new_point(p->x - (2 * projection_x_coord),
		     p->y - (2 * projection_y_coord));
  }
  return rval;
}

static VALUE
rb_line_mirror_point(Line *me, Point *p) {
  Point *rval = line_mirror_point(me, p);
  return RB_POINT(rval, CLASS(p->rbPoint));
}

static VALUE 
rb_line_mirror_line(Line *me, Line *o) {
  Point *p1 = line_mirror_point(me, o->p1);
  Point *p2 = line_mirror_point(me, o->p2);
  Line *rval;
  RB_POINT(p1, CLASS(o->p1->rbPoint));
  RB_POINT(p2, CLASS(o->p2->rbPoint));
  rval = new_line_with_points(p1, p2);
  return RB_LINE(rval, CLASS(o->rbLine));
}

VALUE
rb_line_mirror(VALUE self, VALUE other) {
  Line *me;
  if (LINE_P(other)) {
    Line *o;
    LINE(self, me);
    LINE(other, o);
    return rb_line_mirror_line(me, o);
  } else if (POINT_P(other)) {
    Point *o;
    LINE(self, me);
    POINT(other, o);
    return rb_line_mirror_point(me, o);
  } else {
    rb_raise(rb_eTypeError, "Argument to %s#mirror have to be either Geo::Line or Geo::Point.", rb_obj_classname(self));
  }
}

VALUE
rb_line_side(VALUE self, VALUE p) {
  Line *me;
  Point *point;
  CHECK_POINT(p);
  LINE(self, me);
  POINT(p, point);
  return INT2NUM(line_side(me, point));
}

VALUE
rb_line_contains(VALUE self, VALUE p) {
  Line *me;
  Point *point;
  gint side;
  CHECK_POINT(p);
  LINE(self,me);
  POINT(p, point);
  side = line_side(me, point);
  if (side == 0) {
    return Qtrue;
  } else {
    return Qfalse;
  }
}

VALUE
rb_line_from_gpointer(gpointer l) {
  return ( (Line *) l)->rbLine;
}

gboolean
line_slide_point(Intersection *intersection, Line *line, gint ttl) {
  Point touchPoint;
  Point destOne;
  Point destTwo;
  gdouble distanceLeft;
  gdouble errorOne;
  gdouble errorTwo;
  /*
   * The line we want to adjust against. If the intersection is with a line of zero
   * length, then it is against ourselves. Otherwise with the intersecting line.
   */
  gboolean zero_intersection = intersection->type == itLINE && ZERO_DISTANCE_P(intersection->with->p1, intersection->with->p2);
  gdouble adjustment_length = zero_intersection ? DISTANCE(line->p1, line->p2) : DISTANCE(intersection->with->p1, intersection->with->p2);
  gdouble adjustment_xunit = zero_intersection ?	\
    (line->p2->x - line->p1->x) / adjustment_length :			\
    (intersection->with->p2->x - intersection->with->p1->x) / adjustment_length;
  gdouble adjustment_yunit = zero_intersection ?	\
    (line->p2->y - line->p1->y) / adjustment_length :			\
    (intersection->with->p2->y - intersection->with->p1->y) / adjustment_length;
  /*
   * Calculate where we actually touch the other line.
   */
  if (intersection->type == itLINE) {
    if (DISTANCE(intersection->line->p1, line->p1) <
	DISTANCE(intersection->line->p2, line->p1)) {
      touchPoint = *(intersection->line->p1);
    } else {
      touchPoint = *(intersection->line->p2);
    }
  } else {
    touchPoint = *(intersection->point);
  }
  if (intersection->type == itPOINT) {
    /*
     * If we intersected with a point, move the touch point back towards the start point to not actually touch the other line.
     */
    if (line->p1->x > touchPoint.x) 
      touchPoint.x += MIN(1, line->p1->x - touchPoint.x);
    if (line->p1->x < touchPoint.x)
      touchPoint.x -= MIN(1, touchPoint.x - line->p1->x);
    if (line->p1->y > touchPoint.y)
      touchPoint.y += MIN(1, line->p1->y - touchPoint.y);
    if (line->p1->y < touchPoint.y)
      touchPoint.y -= MIN(1, touchPoint.y - line->p1->y);
  } else {
    /*
     * If we intersected with a line, move perpendicular to our adjustment-line.
     */
    if (rand() < RAND_MAX / 2) {
      touchPoint.x += adjustment_yunit;
      touchPoint.y -= adjustment_xunit;
    } else {
      touchPoint.x -= adjustment_yunit;
      touchPoint.y += adjustment_xunit;
    }
  }
  /*
   * If the ttl is reached then we just set the line's p2 to the touchPoint.
   */
  if (ttl < 1) {
    line->p2->x = touchPoint.x;
    line->p2->y = touchPoint.y;
    /*
     * We are done here, no more sliding.
     */
    return TRUE;
  } else {
    /*
     * Otherwise we calculate two alternate destinations.
     *
     * If we intersected with a zero length line, then we just go ahead in our 
     * original direction, otherwise we follow the intersecting line.
     *
     */
    destOne = touchPoint;
    destTwo = touchPoint;
    distanceLeft = DISTANCE(line->p1, line->p2) - DISTANCE(&touchPoint, line->p1);
    if (zero_intersection) {
      destOne.x += adjustment_xunit * distanceLeft;
      destOne.y += adjustment_yunit * distanceLeft;
      destTwo.x += adjustment_xunit * distanceLeft;
      destTwo.y += adjustment_yunit * distanceLeft;
    } else {
      destOne.x += adjustment_xunit * distanceLeft;
      destOne.y += adjustment_yunit * distanceLeft;
      destTwo.x -= adjustment_xunit * distanceLeft;
      destTwo.y -= adjustment_yunit * distanceLeft;
    }
    /*
     * And select the one with the least error.
     */
    errorOne = DISTANCE(line->p2, &destOne);
    errorTwo = DISTANCE(line->p2, &destTwo);
    if (DBL_EQL(errorOne, errorTwo)) {
      line->p2->x = touchPoint.x;
      line->p2->y = touchPoint.y;
      /*
       * We are done here, no more sliding.
       */
      return TRUE;
    } else {
      if (errorOne < errorTwo) {
	line->p2->x = destOne.x;
	line->p2->y = destOne.y;
      } else if (errorOne > errorTwo) {
	line->p2->x = destTwo.x;
	line->p2->y = destTwo.y;
      }
      line->p1->x = touchPoint.x;
      line->p1->y = touchPoint.y;
      /*
       * We are possibly not done, return FALSE.
       */
      return FALSE;
    }
  }
}

guint
line_hash(gconstpointer l) {
  Line *line = (Line *) l;
  return (guint) (line->p1->x + line->p1->y + line->p2->x + line->p2->y);
}

