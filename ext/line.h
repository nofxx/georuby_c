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


#ifndef LINE_H
#define LINE_H

#include <common.h>

extern VALUE rb_line;

#define RB_LINE(line_pointer,klass) ((line_pointer)->rbLine = Data_Wrap_Struct(klass, line_mark, free, (line_pointer)))
#define LINE(rb_line,line_pointer) Data_Get_Struct((rb_line), Line, (line_pointer))
#define LINE_ANGLE(me) (ANGLE((me)->p2->x - (me)->p1->x,(me)->p2->y - (me)->p1->y))
#define LINE_VERTICAL(line) ((line)->p1->x == (line)->p2->x)
#define LINE_Y(line,v) (LINE_VERTICAL((line)) ? \
			(line)->p1->y : \
			(line)->p1->y + ((line)->p2->y - (line)->p1->y) * ((v - (line)->p1->x) / ((line)->p2->x - (line)->p1->x)))
#define LINE_EQUALS(l1,l2) (POINT_EQUALS(((Line *) (l1))->p1, ((Line *) (l2))->p1) && POINT_EQUALS(((Line *) (l1))->p2, ((Line *) (l2))->p2))
/*
 * Is the point outside the rectangle described by the endpoints of the line?
 */
#define POINT_OUTSIDE_LINE(l,p) (((DBL_GT((p)->x, (l)->p1->x) && DBL_GT((p)->x, (l)->p2->x)) || \
				  (DBL_LT((p)->x, (l)->p1->x) && DBL_LT((p)->x, (l)->p2->x))) && \
				 ((DBL_GT((p)->y, (l)->p1->y) && DBL_GT((p)->y, (l)->p2->y)) || \
				  (DBL_LT((p)->y, (l)->p1->y) && DBL_LT((p)->y, (l)->p2->y))))

#define POINT_RIGHT_OF_LINE(l,p) (DBL_GT((p)->x, (l)->p1->x) && DBL_GT((p)->x, (l)->p2->x))
#define POINT_LEFT_OF_LINE(l,p) (DBL_LT((p)->x, (l)->p1->x) && DBL_LT((p)->x, (l)->p2->x))
#define POINT_ABOVE_LINE(l,p) (DBL_LT((p)->y, (l)->p1->y) && DBL_LT((p)->y, (l)->p2->y))
#define POINT_BELOW_LINE(l,p) (DBL_GT((p)->y, (l)->p1->y) && DBL_GT((p)->y, (l)->p2->y))
#define LINE_RIGHT_OF_LINE(subject,object) (POINT_RIGHT_OF_LINE((object), (subject)->p1) && POINT_RIGHT_OF_LINE((object), (subject)->p2))
#define LINE_LEFT_OF_LINE(subject,object) (POINT_LEFT_OF_LINE((object), (subject)->p1) && POINT_LEFT_OF_LINE((object), (subject)->p2))
#define LINE_ABOVE_LINE(subject,object) (POINT_ABOVE_LINE((object), (subject)->p1) && POINT_ABOVE_LINE((object), (subject)->p2))
#define LINE_BELOW_LINE(subject,object) (POINT_BELOW_LINE((object), (subject)->p1) && POINT_BELOW_LINE((object), (subject)->p2))
/*
 * Is both of the subjects endpoints outside the rectangle described by the endpoints of the object?
 */
#define LINE_OUTSIDE_LINE(subject,object) (LINE_RIGHT_OF_LINE((subject), (object)) || \
					   LINE_LEFT_OF_LINE((subject), (object)) || \
					   LINE_ABOVE_LINE((subject), (object)) || \
					   LINE_BELOW_LINE((subject), (object)))

/*
 * Is the point between the endpoints of the line?
 */
#define POINT_WITHIN_LINE(line_pointer,point_pointer) (DBL_EOG((point_pointer)->x, MIN((line_pointer)->p1->x, (line_pointer)->p2->x)) && \
						       DBL_EOL((point_pointer)->x, MAX((line_pointer)->p1->x, (line_pointer)->p2->x)) && \
						       DBL_EOG((point_pointer)->y, MIN((line_pointer)->p1->y, (line_pointer)->p2->y)) && \
						       DBL_EOL((point_pointer)->y, MAX((line_pointer)->p1->y, (line_pointer)->p2->y)))

#define POINT_ON_LINE(line_pointer,point_pointer) (DBL_EQL((point_pointer)->y, LINE_Y((line_pointer), (point_pointer)->x)) && \
						   POINT_WITHIN_LINE((line_pointer), (point_pointer))
/*
 * Is any of the endpoints of the subject within the endpoints of the object?
 */
#define LINE_WITHIN_LINE(subject,object) (POINT_WITHIN_LINE((subject), (object)->p1) || \
					  POINT_WITHIN_LINE((subject), (object)->p2))
#define LINE_DOT(l1,l2) ((((l1)->p2->x - (l1)->p1->x) * ((l2)->p2->x - (l2)->p1->x)) + (((l1)->p2->y - (l1)->p1->y) * ((l2)->p2->y - (l2)->p1->y)))
#define POINT_BETWEEN_PARALLEL_LINES(l1,l2,p) ((line_side((l1),(p)) >= 0 && line_side((l2),(p)) <= 0) || \
					       (line_side((l1),(p)) <= 0 && line_side((l2),(p)) >= 0))
#define LINE_P(l) (!NIL_P((l)) && rb_is_a((l), rb_line))
#define CHECK_LINE(l) if (!LINE_P((l))) rb_raise(rb_eTypeError, "Expected Geo::Line!")
#define LINE_PARALLEL(l,p) (new_line_with_coordinates((p)->x, (p)->y, (p)->x + (l)->p2->x - (l)->p1->x, (p)->y + (l)->p2->y - (l)->p1->y))
#define LINE_BOUNDS(line,bottom_left,top_right) (bottom_left)->x = MIN(line->p1->x, line->p2->x); \
  (bottom_left)->y = MIN(line->p1->y, line->p2->y);			\
  (top_right)->x = MAX(line->p1->x, line->p2->x);			\
  (top_right)->y = MAX(line->p1->y, line->p2->y)

void
line_mark(Line *line);

Line*
new_line_with_points(Point *p1, Point *p2);

Line*
new_line_with_coordinates(gdouble x1, gdouble y1, gdouble x2, gdouble y2);

void
line_destroy_notify(gpointer l);

VALUE
rb_line_alloc(VALUE class);

VALUE
rb_line_initialize(int argc, VALUE *argv, VALUE self);

VALUE
rb_line_inspect(VALUE self);

VALUE
rb_line_set_p1(VALUE self, VALUE new_point);

VALUE
rb_line_set_p2(VALUE self, VALUE new_point);

VALUE
rb_line_p1(VALUE self);

VALUE
rb_line_p2(VALUE self);

/*
 * If argument is a point:
 * Is the point between the endpoints of the line?
 * If argument is a line:
 * Is any of the endpoints of the subject within the endpoints of the object?
 */
VALUE
rb_line_within(VALUE self, VALUE arg);

/*
 * If argument is a point:
 * Is the point outside the rectangle described by the endpoints of the line?
 * If argument is a line:
 * Is both of the subjects endpoints outside the rectangle described by the endpoints of the object?
 */
VALUE
rb_line_outside(VALUE self, VALUE arg);


VALUE
rb_line_parallel(VALUE self, VALUE p);

gint
line_side(Line *me, Point *point);

Intersection*
line_intersection(Line *l1, Line *l2);

VALUE
rb_line_intersection(VALUE self, VALUE other_line);

VALUE
rb_line_dot(VALUE self, VALUE o);

VALUE
rb_line_abs(VALUE self);

VALUE
rb_line_equals(VALUE self, VALUE o);

VALUE
rb_line_cmp(VALUE self, VALUE o);

VALUE
rb_line_reverse(VALUE self);

VALUE
rb_line_set_angle(VALUE self, VALUE new_angle);

VALUE
rb_line_angle(VALUE self);

VALUE
rb_line_set_abs(VALUE self, VALUE new_abs);

VALUE
rb_line_clone(VALUE self);

VALUE
rb_line_side(VALUE self, VALUE p);

VALUE
rb_line_contains(VALUE self, VALUE p);

VALUE
rb_line_from_gpointer(gpointer l);

void
line_set_abs(Line *me, double new_abs);

gboolean
line_slide_point(Intersection *intersection, Line *line, gint ttl);

gchar *
line_inspect(Line *l);

guint
line_hash(gconstpointer l);

gboolean
line_equals(gconstpointer a, gconstpointer b);

void
line_free(Line *l);

VALUE
rb_line_mirror(VALUE self, VALUE other);

#endif
