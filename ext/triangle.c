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

VALUE rb_triangle;

static void
triangle_mark(Triangle *triangle) {
  if (triangle->p1 != NULL)
    rb_gc_mark(triangle->p1->rbPoint);
  if (triangle->p2 != NULL)
    rb_gc_mark(triangle->p2->rbPoint);
  if (triangle->p3 != NULL)
    rb_gc_mark(triangle->p3->rbPoint);
}

static Triangle*
new_triangle_with_coordinates(gdouble x1, gdouble y1, gdouble x2, gdouble y2, gdouble x3, gdouble y3) {
  Triangle *rval = ALLOC(Triangle);
  rval->p1 = new_point(x1,y1);
  rval->p2 = new_point(x2,y2);
  rval->p3 = new_point(x3,y3);
  rval->rbTriangle = 0;
  return rval;
}

VALUE
rb_triangle_alloc(VALUE class) {
  Triangle *t = ALLOC(Triangle);
  t->p1 = NULL;
  t->p2 = NULL;
  t->p3 = NULL;
  return RB_TRIANGLE(t, class);
}

VALUE
rb_triangle_initialize(int argc, VALUE *argv, VALUE self) {
  Triangle *t;
  Point *p1;
  Point *p2;
  Point *p3;
  if (argc == 3) {
    CHECK_POINT(argv[0]);
    CHECK_POINT(argv[1]);
    CHECK_POINT(argv[2]);
    POINT(argv[0], p1);
    POINT(argv[1], p2);
    POINT(argv[2], p3);
  } else if (argc == 6) {
    CHECK_NUMERICALITY(argv[0]);
    CHECK_NUMERICALITY(argv[1]);
    CHECK_NUMERICALITY(argv[2]);
    CHECK_NUMERICALITY(argv[3]);
    CHECK_NUMERICALITY(argv[4]);
    CHECK_NUMERICALITY(argv[5]);
    p1 = ALLOC(Point);
    p1->x = NUM2DBL(argv[0]);
    p1->y = NUM2DBL(argv[1]);
    p2 = ALLOC(Point);
    p2->x = NUM2DBL(argv[2]);
    p2->y = NUM2DBL(argv[3]);
    p3 = ALLOC(Point);
    p3->x = NUM2DBL(argv[4]);
    p3->y = NUM2DBL(argv[5]);
    RB_POINT(p1, rb_point);
    RB_POINT(p2, rb_point);
    RB_POINT(p3, rb_point);
  } else {
    rb_raise(rb_eTypeError, "Arguments to %s#new have to be 3 Geo::Points or 6 numbers.", rb_obj_classname(self));
  }
  TRIANGLE(self, t);
  t->p1 = p1;
  t->p2 = p2;
  t->p3 = p3;
  return self;
}

gchar*
triangle_inspect(Triangle *t) {
  gchar *rval = calloc(1024, sizeof(gchar));
  gchar *p1 = point_inspect(t->p1);
  gchar *p2 = point_inspect(t->p2);
  gchar *p3 = point_inspect(t->p3);
  snprintf(rval, 1024, "<%s:%p p1=%s p2=%s p3=%s>",
	   rb_obj_classname(t->rbTriangle), t, 
	   p1,
	   p2,
	   p3);
  free(p1);
  free(p2);
  free(p3);
  return rval;
}

VALUE
rb_triangle_inspect(VALUE self) {
  Triangle *t;
  gchar *str;
  VALUE rval;
  TRIANGLE(self, t);
  str = triangle_inspect(t);
  rval = rb_str_new2(str);
  free(str);
  return rval;
}

VALUE
rb_triangle_set_p1(VALUE self, VALUE new_point) {
  Triangle *t;
  Point *p;
  CHECK_POINT(new_point);
  TRIANGLE(self, t);
  POINT(new_point, p);
  t->p1 = p;
  return new_point;
}

VALUE
rb_triangle_set_p2(VALUE self, VALUE new_point) {
  Triangle *t;
  Point *p;
  CHECK_POINT(new_point);
  TRIANGLE(self, t);
  POINT(new_point, p);
  t->p2 = p;
  return new_point;
}

VALUE
rb_triangle_set_p3(VALUE self, VALUE new_point) {
  Triangle *t;
  Point *p;
  CHECK_POINT(new_point);
  TRIANGLE(self, t);
  POINT(new_point, p);
  t->p3 = p;
  return new_point;
}

VALUE
rb_triangle_p1(VALUE self) {
  Triangle *t;
  TRIANGLE(self, t);
  return t->p1->rbPoint;
}

VALUE
rb_triangle_p2(VALUE self) {
  Triangle *t;
  TRIANGLE(self, t);
  return t->p2->rbPoint;
}

VALUE
rb_triangle_p3(VALUE self) {
  Triangle *t;
  TRIANGLE(self, t);
  return t->p3->rbPoint;
}

static Line**
triangle_lines(Triangle *triangle) {
  Line **rval = malloc(sizeof(Line) * 3);
  rval[0] = new_line_with_points(triangle->p1, triangle->p2);
  rval[1] = new_line_with_points(triangle->p1, triangle->p3);
  rval[2] = new_line_with_points(triangle->p2, triangle->p3);
  return rval;
}

static void
free_triangle_lines(Line **lines) {
  free(lines[0]);
  free(lines[1]);
  free(lines[2]);
}

gboolean
triangle_contains(Triangle *triangle, Point *point) {
  Line **lines = triangle_lines(triangle);
  Line *l12p = LINE_PARALLEL(lines[0], triangle->p3);
  Line *l13p = LINE_PARALLEL(lines[1], triangle->p2);
  Line *l23p = LINE_PARALLEL(lines[2], triangle->p1);
  gint rval = POINT_BETWEEN_PARALLEL_LINES(lines[0], l12p, point) && POINT_BETWEEN_PARALLEL_LINES(lines[1], l13p, point) && POINT_BETWEEN_PARALLEL_LINES(lines[2], l23p, point);
  line_free(l12p);
  line_free(l13p);
  line_free(l23p);
  free_triangle_lines(lines);
  return rval;
}

static Intersection**
triangle_intersections(Triangle *triangle, Line *line) {
  Intersection **rval = malloc(sizeof(Intersection) * 3);
  Line **lines = triangle_lines(triangle);
  rval[0] = line_intersection(lines[0], line);
  rval[1] = line_intersection(lines[1], line);
  rval[2] = line_intersection(lines[2], line);
  free_triangle_lines(lines);
  return rval;
}

static Intersection*
triangle_closest_intersection(Triangle *triangle, Line *line, Point *point) {
  Intersection *rval;
  Intersection **intersections = triangle_intersections(triangle, line);
  gdouble distance12 = intersections[0] == NULL ? -1 : INTERSECTION_DISTANCE(intersections[0], point);
  gdouble distance13 = intersections[1] == NULL ? -1 : INTERSECTION_DISTANCE(intersections[1], point);
  gdouble distance23 = intersections[2] == NULL ? -1 : INTERSECTION_DISTANCE(intersections[2], point);
  if (intersections[0] != NULL && 
      (intersections[1] == NULL || distance12 <= distance13) && 
      (intersections[2] == NULL || distance12 <= distance23)) {
    if (intersections[1] != NULL)
      free_intersection(intersections[1]);
    if (intersections[2] != NULL)
      free_intersection(intersections[2]);
    rval = intersections[0];
  } else if (intersections[1] != NULL && 
	     (intersections[0] == NULL || distance13 <= distance12) && 
	     (intersections[2] == NULL || distance13 <= distance23)) {
    if (intersections[0] != NULL)
      free_intersection(intersections[0]);
    if (intersections[2] != NULL)
      free_intersection(intersections[2]);
    rval = intersections[1];
  } else if (intersections[2] != NULL && 
	     (intersections[0] == NULL || distance23 <= distance12) && 
	     (intersections[1] == NULL || distance23 <= distance13)) {
    if (intersections[0] != NULL)
      free_intersection(intersections[0]);
    if (intersections[1] != NULL)
      free_intersection(intersections[1]);
    rval = intersections[2];
  } else {
    if (intersections[0] != NULL)
      free_intersection(intersections[0]);
    if (intersections[1] != NULL)
      free_intersection(intersections[1]);
    if (intersections[2] != NULL)
      free_intersection(intersections[2]);
    rval = NULL;
  }
  return rval;
}

Line*
triangle_overlap(Triangle *triangle, Line *line) {
  if (triangle_contains(triangle, line->p1)) {
    if (triangle_contains(triangle, line->p2)) {
      return new_line_with_coordinates(line->p1->x, line->p1->y, line->p2->x, line->p2->y);
    } else {
      Intersection *closest_to_p2 = triangle_closest_intersection(triangle, line, line->p2);
      if (closest_to_p2 != NULL) {
	Point *closest_of_intersection = INTERSECTION_CLOSEST_POINT(closest_to_p2, line->p2);
	Line *rval = new_line_with_coordinates(line->p1->x, line->p1->y, closest_of_intersection->x, closest_of_intersection->y);
	free_intersection(closest_to_p2);
	return rval;
      } else {
	return NULL;
      }
    }
  } else if (triangle_contains(triangle, line->p2)) {
    Intersection *closest_to_p1 = triangle_closest_intersection(triangle, line, line->p1);
    if (closest_to_p1 != NULL) {
      Point *closest_of_intersection = INTERSECTION_CLOSEST_POINT(closest_to_p1, line->p1);
      Line *rval = new_line_with_coordinates(closest_of_intersection->x, closest_of_intersection->y, line->p2->x, line->p2->y);
      free_intersection(closest_to_p1);
      return rval;
    } else {
      return NULL;
    }
  }
  Intersection *closest_to_p1 = triangle_closest_intersection(triangle, line, line->p1);
  if (closest_to_p1 != NULL) {
    Intersection *closest_to_p2 = triangle_closest_intersection(triangle, line, line->p2);
    Point *closest_of_closest_to_p1 = INTERSECTION_CLOSEST_POINT(closest_to_p1, line->p1);
    Point *closest_of_closest_to_p2 = INTERSECTION_CLOSEST_POINT(closest_to_p2, line->p2);
    Line *rval = new_line_with_coordinates(closest_of_closest_to_p1->x, closest_of_closest_to_p1->y, closest_of_closest_to_p2->x, closest_of_closest_to_p2->y);
    free_intersection(closest_to_p2);
    free_intersection(closest_to_p1);
    return rval;
  }
  return NULL;
}

VALUE
rb_triangle_overlap(VALUE self, VALUE line) {
  Triangle *me;
  Line *l;
  Line *rval;
  CHECK_LINE(line);
  TRIANGLE(self, me);
  LINE(line, l);
  rval = triangle_overlap(me, l);
  if (rval != NULL) {
    RB_POINT(rval->p1, CLASS(l->p1->rbPoint));
    RB_POINT(rval->p2, CLASS(l->p2->rbPoint));
    return RB_LINE(rval, CLASS(line));
  }
  return Qnil;
}

gboolean
triangle_intersects(Triangle *triangle, Line *line) {
  Line **lines = triangle_lines(triangle);
  Intersection *intersection;
  gint rval = 0;
  if ((intersection = line_intersection(lines[0], line)) != NULL) {
    free_intersection(intersection);
    rval = 1;
  } else if ((intersection = line_intersection(lines[1], line)) != NULL) {
    free_intersection(intersection);
    rval = 1;
  } else if ((intersection = line_intersection(lines[2], line)) != NULL) {
    free_intersection(intersection);
    rval = 1;
  }
  free_triangle_lines(lines);
  return rval;
}

VALUE
rb_triangle_equals(VALUE self, VALUE o) {
  if (!TRIANGLE_P(o)) {
    return Qfalse;
  } else {
    Triangle *me;
    Triangle *other;
    TRIANGLE(self, me);
    TRIANGLE(o, other);
    if (TRIANGLE_EQUALS(me, other)) {
      return Qtrue;
    } else {
      return Qfalse;
    }
  }
}

static gdouble
triangle_area(Triangle *me) {
  Line *l1;
  Line *l2;
  gdouble length1;
  gdouble length2;
  l1 = new_line_with_points(me->p1, me->p2);
  l2 = new_line_with_points(me->p1, me->p3);
  length1 = DISTANCE(l1->p1, l1->p2);
  length2 = DISTANCE(l2->p1, l2->p2);
  return sin(acos(LINE_DOT(l1,l2) / (length1 * length2))) * length1 * length2 / 2.0;
}

static gint
triangle_cmp(Triangle *me, Triangle *other) {
  gint tmp;
  gdouble my_area;
  gdouble other_area;
  my_area = triangle_area(me);
  other_area = triangle_area(other);
  if (!DBL_EQL(my_area, other_area)) 
    return CMP(my_area, other_area);
  else if ((tmp = point_cmp(me->p1, other->p1)) != 0)
    return tmp;
  else if ((tmp = point_cmp(me->p2, other->p2)) != 0)
    return tmp;
  else
    return point_cmp(me->p3, other->p3);
}

VALUE
rb_triangle_cmp(VALUE self, VALUE o) {
  if (!TRIANGLE_P(o)) {
    return INT2NUM(0);
  } else {
    Triangle *me;
    Triangle *other;
    TRIANGLE(self, me);
    TRIANGLE(o, other);
    return INT2NUM(triangle_cmp(me, other));
  }
}

VALUE
rb_triangle_clone(VALUE self) {
  Triangle *me;
  Triangle *rval;
  TRIANGLE(self, me);
  rval = new_triangle_with_coordinates(me->p1->x, me->p1->y, me->p2->x, me->p2->y, me->p3->x, me->p3->y);
  RB_POINT(rval->p1, CLASS(me->p1->rbPoint));
  RB_POINT(rval->p2, CLASS(me->p2->rbPoint));
  RB_POINT(rval->p3, CLASS(me->p3->rbPoint));
  return RB_TRIANGLE(rval, CLASS(self));
}

VALUE
rb_triangle_area(VALUE self) {
  Triangle *me;
  TRIANGLE(self, me);
  return rb_float_new(triangle_area(me));
}

VALUE
rb_triangle_contains(VALUE self, VALUE p) {
  Triangle *me;
  Point *point;
  CHECK_POINT(p);
  TRIANGLE(self, me);
  POINT(p, point);
  if (triangle_contains(me, point)) {
    return Qtrue;
  } else {
    return Qfalse;
  }
}

VALUE
rb_triangle_intersects(VALUE self, VALUE line) {
  Triangle *me;
  Line *l;
  CHECK_LINE(line);
  TRIANGLE(self, me);
  LINE(line, l);
  if (triangle_intersects(me, l)) {
    return Qtrue;
  } else {
    return Qfalse;
  }
}

VALUE
rb_triangle_from_gpointer(gpointer l) {
  return ( (Triangle *) l)->rbTriangle;
}

