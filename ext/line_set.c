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

VALUE rb_line_set;

static gboolean
line_set_reindex(GeoSet *line_set);

static void
g_hash_table_mark_line(gpointer key, gpointer value, gpointer user_data) {
  rb_gc_mark( ( (Line *) key )->rbLine);
}

void
line_set_mark(GeoSet *set) {
  g_hash_table_foreach(set->table, g_hash_table_mark_line, NULL);
}

VALUE
rb_line_set_alloc(VALUE class) {
  GeoSet *set = new_geo_set();
  return RB_LINE_SET(set, class);
}

VALUE
rb_line_set_insert(VALUE self, VALUE line) {
  GeoSet *me;
  Line *l;
  CHECK_LINE(line);
  GEO_SET(self, me);
  LINE(line, l);
  geo_set_insert(me, (gpointer ) l);
  return self;
}

VALUE
rb_line_set_include(VALUE self, VALUE line) {
  GeoSet *me;
  Line *t;
  CHECK_LINE(line);
  LINE(line, t);
  GEO_SET(self, me);
  return GBOOL2RB(g_hash_table_lookup_extended(me->table, (gpointer) t, NULL, NULL));
}

VALUE
rb_line_set_delete(VALUE self, VALUE line) {
  GeoSet *me;
  Line *t;
  CHECK_LINE(line);
  GEO_SET(self, me);
  LINE(line, t);
  return geo_set_delete(me, (gpointer) t, line);
}

static gpointer
g_hash_table_yield_line(GeoSet *set, gpointer structure, gpointer user_data) {
  rb_yield(( (Line *) structure )->rbLine);
  return NULL;
}

VALUE
rb_line_set_each(VALUE self) {
  GeoSet *set;
  GEO_SET(self, set);
  geo_set_each_structure_until(set, g_hash_table_yield_line, NULL);
  return self;
}

static gpointer
line_set_intersection(GeoSet *set, gpointer set_line_gp, gpointer line_gp) {
  Intersection *intersection;
  Line *set_line = (Line *) set_line_gp;
  Line *line = (Line *) line_gp;
  if ((intersection = line_intersection(set_line, line)) != NULL)
    return intersection;
  return NULL;
}

gboolean
line_set_intersects(GeoSet *set, Line *line) {
  Intersection *intersection = NULL;
  if (set->indexed) {
    LINE_SET_REINDEX(set);
    intersection = geo_set_each_structure_having_common_segment_id_with_line_until(set, line, line_set_intersection, line);
  } else {
    intersection = geo_set_each_structure_until(set, line_set_intersection, line);
  }
  if (intersection != NULL)
    free_intersection(intersection);
  return GBOOL2RB(intersection != NULL);
}

VALUE
rb_line_set_intersects(VALUE self, VALUE l) {
  GeoSet *set;
  Line *line;
  CHECK_LINE(l);
  GEO_SET(self, set);
  LINE(l, line);
  if (line_set_intersects(set, line)) {
    return Qtrue;
  } else {
    return Qfalse;
  }
}

static gpointer
line_set_closest_intersection_in_segments(GeoSet *set, gpointer segment_line_gp, gpointer user_data) {
  gpointer *args = (gpointer *) user_data;
  gdouble *closest_distance = (gdouble *) args[0];
  Intersection **closest_intersection = (Intersection **) args[1];
  Line *line = (Line *) args[2];
  Line *segment_line = (Line *) segment_line_gp;
  Intersection *this;
  gdouble this_distance;
  if ((this = line_intersection(segment_line, line)) != NULL) {
    this_distance = INTERSECTION_DISTANCE(this, line->p1);
    if (*closest_intersection == NULL) {
      *closest_intersection = this;
      *closest_distance = this_distance;
    } else if (this_distance < *closest_distance) {
      free_intersection(*closest_intersection);
      *closest_intersection = this;
      *closest_distance = this_distance;
    } else {
      free_intersection(this);
    }
  }
  return NULL;
}

static Intersection*
line_set_closest_intersection(GeoSet *set, Line *line) {
  Intersection *closest = NULL;
  double closest_distance = -1;
  gpointer args[3] = { &closest_distance, &closest, line };
  if (set->indexed) {
    LINE_SET_REINDEX(set);
    geo_set_each_structure_having_common_segment_id_with_line_until(set, line, line_set_closest_intersection_in_segments, args);
  } else {
    geo_set_each_structure_until(set, line_set_closest_intersection_in_segments, args);
  }
  return closest;
}

void
line_set_slide_point(GeoSet *set, Line *line, gint ttl) {
  gboolean done = FALSE;
  Intersection *intersection;
  ZERO_DISTANCE_CHECK(line->p1, line->p2);
  while ((intersection = line_set_closest_intersection(set, line)) != NULL && ttl > 0 && !done) {
    done = line_slide_point(intersection, line, ttl);
    free_intersection(intersection);
    ttl--;
  }
}

VALUE
rb_line_set_slide(VALUE self, VALUE l) {
  GeoSet *set;
  Line *line;
  CHECK_LINE(l);
  GEO_SET(self, set);
  LINE(l, line);
  line_set_slide_point(set, line, 3);
  return line->p2->rbPoint;
}

static void
g_hash_table_each_intersection_free_endpoint(gpointer key, gpointer data, gpointer user_data) {
  gpointer *arg_wrapper = (gpointer *) user_data;
  
  gpointer *original_args = (gpointer *) arg_wrapper[0];
  gdouble *max_distance = (gdouble *) original_args[1];
  Line *tmpLine = (Line *) original_args[2];
  GeoSet *line_set = (GeoSet *) original_args[3];
  geo_set_structure_handler handler = (geo_set_structure_handler) original_args[4];
  gpointer handler_args = original_args[5];
  GHashTable *seen_points = (GHashTable *) original_args[6];

  tmpLine->p1 = (Point *) arg_wrapper[1];
  tmpLine->p2 = (Point *) key;
  
  if (*max_distance < 0 || DISTANCE(tmpLine->p1, tmpLine->p2) < *max_distance) {
    if (!line_set_intersects(line_set, tmpLine)) {
      if (g_hash_table_lookup(seen_points, key) == NULL) {
	handler(line_set, tmpLine, handler_args);
	g_hash_table_insert(seen_points, key, GINT_TO_POINTER(1));
      }
    }
  }
}

static gpointer
line_set_insert_p2_into_rb_ary(GeoSet *line_set, gpointer line, gpointer rval) {
  rb_ary_push(*( (VALUE *) rval ), ((Line *) line)->p2->rbPoint);
  return NULL;
}

static void
g_hash_table_each_intersection_free_endpoint_for_point(gpointer key, gpointer data, gpointer user_data) {
  gpointer *given_args = (gpointer *) user_data;
  GeoSet *endpoints = (GeoSet *) given_args[0];
  gpointer my_args[2] = { user_data, key };
  g_hash_table_foreach(endpoints->table, g_hash_table_each_intersection_free_endpoint, (gpointer) my_args);
}
 
void
line_set_each_intersection_free_endpoint(GeoSet *set, 
					 GeoSet *origins, 
					 GeoSet *endpoints, 
					 gdouble max_distance, 
					 geo_set_structure_handler handler,
					 gpointer user_data) {
  Line line;
  GHashTable *seen_points = g_hash_table_new(g_direct_hash, g_direct_equal);
  gpointer args[7] = { endpoints, &max_distance, &line, set, handler, user_data, seen_points };
  g_hash_table_foreach(origins->table, g_hash_table_each_intersection_free_endpoint_for_point, args);
  g_hash_table_destroy(seen_points);
}

static gpointer
line_set_count_intersections(GeoSet *set, gpointer segment_line_gp, gpointer user_data) {
  gpointer *args = (gpointer *) user_data;
  Line *segment_line = (Line *) segment_line_gp;
  gint *rval = (gint *) args[0];
  Line *line = (Line *) args[1];
  Intersection *intersection = line_intersection(line, segment_line);
  if (intersection != NULL) {
    free_intersection(intersection);
    (*rval)++;
  }
  return NULL;
}
				 
static gint
line_set_n_intersections_with_line(GeoSet *set, Line *line) {
  gint rval = 0;
  gpointer args[2] = { &rval, line };
  if (set->indexed) {
    LINE_SET_REINDEX(set);
    geo_set_each_structure_having_common_segment_id_with_line_until(set, line, line_set_count_intersections, args);
  } else {
    geo_set_each_structure_until(set, line_set_count_intersections, args);
  }
  return rval;
}

VALUE
rb_line_set_n_intersections(VALUE self, VALUE line) {
  GeoSet *me;
  Line *l;
  CHECK_LINE(line);
  GEO_SET(self, me);
  LINE(line, l);
  return INT2NUM(line_set_n_intersections_with_line(me, l));
}

static gpointer
line_set_collect_intersections_in_rb_ary(GeoSet *set, gpointer segment_line_gp, gpointer user_data) {
  gpointer *args = (gpointer *) user_data;
  Line *segment_line = (Line *) segment_line_gp;
  Line *line = (Line *) args[0];
  VALUE *rval = (VALUE *) args[1];
  Intersection *intersection = line_intersection(segment_line, line);
  if (intersection != NULL) {
    rb_ary_push(*rval, rb_ary_new3(2, rb_intersection(intersection, line->p1), intersection->with->rbLine));
  }
  return NULL;
}

static VALUE
line_set_intersections(GeoSet *set, Line *line) {
  VALUE rval = rb_ary_new();
  gpointer args[2] = { line, &rval };
  if (set->indexed) {
    LINE_SET_REINDEX(set);
    geo_set_each_structure_having_common_segment_id_with_line_until(set, line, line_set_collect_intersections_in_rb_ary, args);
  } else {
    geo_set_each_structure_until(set, line_set_collect_intersections_in_rb_ary, args);
  }
  return rval;
}

VALUE
rb_line_set_intersections(VALUE self, VALUE l) {
  GeoSet *me;
  Line *line;
  CHECK_LINE(l);
  GEO_SET(self, me);
  LINE(l, line);
  return line_set_intersections(me, line);
}

VALUE
rb_line_set_intersection_free_endpoints(int argc, VALUE *argv, VALUE self) {
  GeoSet *me;
  GeoSet *ops;
  GeoSet *eps;
  VALUE origins;
  VALUE endpoints;
  VALUE rval;
  gdouble max_distance;
  gboolean created_ops = FALSE;
  if (argc == 2) {
    origins = argv[0];
    endpoints = argv[1];
    max_distance = -1;
  } else if (argc == 3) {
    origins = argv[0];
    endpoints = argv[1];
    max_distance = NUM2DBL(argv[2]);
  } else {
    rb_raise(rb_eTypeError, "Arguments to %s#intersection_free_endpoints have to be (Geo::Point | Geo::PointSet, Geo::PointSet [, Number])", rb_obj_classname(self));
  }
  if (POINT_P(origins)) {
    ops = new_geo_set();
    Point *p;
    POINT(origins, p);
    geo_set_insert(ops, p);
    created_ops = TRUE;
  } else if (POINT_SET_P(origins)) {
    GEO_SET(origins, ops);
  } else {
    rb_raise(rb_eTypeError, "Arguments to %s#intersection_free_endpoints have to be (Geo::Point | Geo::PointSet, Geo::PointSet [, Number])", rb_obj_classname(self));
  }
  CHECK_POINT_SET(endpoints);
  GEO_SET(self, me);
  GEO_SET(endpoints, eps);
  rval = rb_ary_new();
  line_set_each_intersection_free_endpoint(me, ops, eps, max_distance, line_set_insert_p2_into_rb_ary, &rval);
  if (created_ops)
    geo_set_free(ops);
  return rval;
}

VALUE
rb_line_set_closest_intersection(VALUE self, VALUE l) {
  GeoSet *set;
  Line *line;
  Line *with;
  Intersection *intersection;
  VALUE rval;
  CHECK_LINE(l);
  GEO_SET(self, set);
  LINE(l, line);
  if ((intersection = line_set_closest_intersection(set, line)) != NULL) {
    with = intersection->with;
    rval = rb_ary_new3(2, rb_intersection(intersection, line->p1), with->rbLine);
  } else {
    rval = Qnil;
  }
  return rval;
}

VALUE
rb_line_set_clone(VALUE self) {
  GeoSet *me;
  GeoSet *rval;
  GEO_SET(self, me);
  rval = geo_set_clone(me);
  return RB_LINE_SET(rval, CLASS(self));
}

VALUE
rb_line_set_structures_in_segment(VALUE self, VALUE x, VALUE y) {
  return rb_geo_set_structures_in_segment(self, x, y, rb_line_from_gpointer);
}

void
g_hash_table_clone_line_value_into_rb_value(gpointer key, gpointer value, gpointer user_data) {
  Line *l = (Line *) value;
  Line *new_line = new_line_with_coordinates(l->p1->x, l->p1->y, l->p2->x, l->p2->y);
  RB_POINT(new_line->p1, rb_point);
  RB_POINT(new_line->p2, rb_point);
  rb_ary_push(*( (VALUE *) user_data ), RB_LINE(new_line, rb_line));
}

/*
 * Below is the hell you have to go through to get the segment index to work or line sets.
 */

static void
g_hash_table_call_handle_bounds(gpointer key, gpointer value, gpointer user_data) {
  Point bottom_left;
  Point top_right;
  Line *line = (Line *) key;
  LINE_BOUNDS(line, &bottom_left, &top_right);
  geo_set_handle_bounds((GeoSet *) user_data, &bottom_left, &top_right);
}

static void
line_set_bound_finder(GeoSet *set) {
  g_hash_table_foreach(set->table, g_hash_table_call_handle_bounds, set);
}

gpointer
geo_set_each_segment_id_of_intersection_until(GeoSet *set, Line *segment_line, gpointer user_data) {
  gpointer *args = (gpointer *) user_data;
  Line *line = (Line *) args[0];
  geo_set_segment_id_handler handler = (geo_set_segment_id_handler) args[1];
  gpointer handler_args = args[2];
  GHashTable *seen_segment_ids = args[3];
  Intersection *intersection;
  gpointer rval = NULL;
  if ((intersection = line_intersection(line, segment_line)) != NULL) {
    if (intersection->type == itPOINT) {
      rval = geo_set_each_segment_id_for_point_until(set, intersection->point, seen_segment_ids, handler, handler_args);
    } else {
      rval = geo_set_each_segment_id_for_point_until(set, intersection->line->p1, seen_segment_ids, handler, handler_args);
      rval = geo_set_each_segment_id_for_point_until(set, intersection->line->p2, seen_segment_ids, handler, handler_args);
    }
    free_intersection(intersection);
  }
  return rval;
}

/*
 * Will call handler with set, segment id and user_data for each segment id of line in set.
 */
gpointer
geo_set_each_segment_id_of_line_until(GeoSet *set, Line *line, geo_set_segment_id_handler handler, gpointer user_data) {
  Point bottom_left;
  Point top_right;
  GHashTable *seen_segment_ids = g_hash_table_new(g_direct_hash, g_direct_equal);
  gpointer args[4] = { line, handler, user_data, seen_segment_ids };
  gpointer rval = NULL;
  LINE_BOUNDS(line, &bottom_left, &top_right);
  rval = geo_set_each_segment_line_until(set, &bottom_left, &top_right, geo_set_each_segment_id_of_intersection_until, args);
  if (rval == NULL)
    rval = geo_set_each_segment_id_for_point_until(set, line->p1, seen_segment_ids, handler, user_data);
  if (rval == NULL)
    rval = geo_set_each_segment_id_for_point_until(set, line->p2, seen_segment_ids, handler, user_data);
  g_hash_table_destroy(seen_segment_ids);
  return rval;
}

gpointer
geo_set_each_structure_having_common_segment_id_with_line_until(GeoSet *set, 
								Line *line, 
								geo_set_structure_handler handler, 
								gpointer user_data) {
  gpointer rval = NULL;
  GHashTable *seen_structures = g_hash_table_new(g_direct_hash, g_direct_equal);
  gpointer args[3] = { seen_structures, handler, user_data };
  rval = geo_set_each_segment_id_of_line_until(set, line, geo_set_each_structure_in_segment_id_until, args);
  g_hash_table_destroy(seen_structures);
  return rval;
}

VALUE
rb_line_set_lines_with_common_segment_id(VALUE self, VALUE l) {
  GeoSet *me;
  Line *line;
  VALUE rval;
  gpointer args[2];
  CHECK_LINE(l);
  GEO_SET(self, me);
  LINE(l, line);
  LINE_SET_REINDEX(me);
  rval = rb_ary_new();
  args[0] = &rval;
  args[1] = rb_line_from_gpointer;
  geo_set_each_structure_having_common_segment_id_with_line_until(me, line, geo_set_insert_structure_into_rb_ary, args);
  return rval;
}

static void
g_hash_table_each_segment_id_for_line(gpointer key, gpointer value, gpointer user_data) {
  gpointer *args = (gpointer *) user_data;
  geo_set_each_segment_id_of_line_until((GeoSet *) args[0], 
					 (Line *) key, 
					 (geo_set_segment_id_handler) args[1], 
					 (Line *) key);
}
 
static void
line_set_segment_finder(GeoSet *set) {
  gpointer args[2] = { set, geo_set_insert_into_segment };
  g_hash_table_foreach(set->table, g_hash_table_each_segment_id_for_line, args);
}

static gboolean
line_set_reindex(GeoSet *line_set) {
  return geo_set_reindex(line_set, line_set_bound_finder, line_set_segment_finder);
}
 
VALUE
rb_line_set_segment_ids_for_line(VALUE self, VALUE l) {
  GeoSet *me;
  Line *line;
  VALUE rval;
  CHECK_LINE(l);
  GEO_SET(self, me);
  LINE(l, line);
  LINE_SET_REINDEX(me);
  rval = rb_ary_new();
  geo_set_each_segment_id_of_line_until(me, line, geo_set_insert_segment_id_into_rb_ary, &rval);
  return rval;
}

VALUE 
rb_line_set_reindex(VALUE self) {
  GeoSet *me;
  GEO_SET(self, me);
  return GBOOL2RB(line_set_reindex(me));
}

