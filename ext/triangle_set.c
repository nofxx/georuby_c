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

VALUE rb_triangle_set;

static gboolean
triangle_set_reindex(GeoSet *triangle_set);

static void
g_hash_table_mark_triangle(gpointer key, gpointer value, gpointer user_data) {
  rb_gc_mark( ( (Triangle *) key )->rbTriangle);
}

void
triangle_set_mark(GeoSet *set) {
  g_hash_table_foreach(set->table, g_hash_table_mark_triangle, NULL);
}

VALUE
rb_triangle_set_alloc(VALUE class) {
  GeoSet *set = new_geo_set();
  return RB_TRIANGLE_SET(set, class);
}

VALUE
rb_triangle_set_insert(VALUE self, VALUE t) {
  Triangle *triangle;
  GeoSet *me;
  CHECK_TRIANGLE(t);
  GEO_SET(self, me);
  TRIANGLE(t, triangle);
  geo_set_insert(me, (gpointer ) triangle);
  return self;
}

VALUE
rb_triangle_set_include(VALUE self, VALUE triangle) {
  GeoSet *me;
  Triangle *t;
  CHECK_TRIANGLE(triangle);
  TRIANGLE(triangle, t);
  GEO_SET(self, me);
  return GBOOL2RB(g_hash_table_lookup_extended(me->table, (gpointer) t, NULL, NULL));
}

VALUE
rb_triangle_set_delete(VALUE self, VALUE triangle) {
  GeoSet *me;
  Triangle *t;
  CHECK_TRIANGLE(triangle);
  GEO_SET(self, me);
  TRIANGLE(triangle, t);
  return geo_set_delete(me, (gpointer) t, triangle);
}

static gpointer
g_hash_table_yield_triangle(GeoSet *set, gpointer structure, gpointer user_data) {
  rb_yield(( (Triangle *) structure )->rbTriangle);
  return NULL;
}

VALUE
rb_triangle_set_each(VALUE self) {
  GeoSet *set;
  GEO_SET(self, set);
  geo_set_each_structure_until(set, g_hash_table_yield_triangle, NULL);
  return self;
}

static gpointer
triangle_set_each_intersector_for_triangle(GeoSet *set, gpointer structure, gpointer user_data) {
  gpointer *args = (gpointer *) user_data;
  Triangle *triangle = (Triangle *) structure;
  Line *line = (Line *) args[0];
  geo_set_structure_handler handler = (geo_set_structure_handler) args[1];
  gpointer handler_args = args[2];
  if (triangle_intersects(triangle, line)) {
    handler(set, triangle, handler_args);
  }
  return NULL;
}

static gpointer
triangle_set_add_to_rb_ary(GeoSet *set, gpointer structure, gpointer user_data) {
  rb_ary_push(*( (VALUE *) user_data), ( (Triangle *) structure)->rbTriangle);
  return NULL;
}

gpointer
triangle_set_each_intersector_until(GeoSet *set, Line *line, geo_set_structure_handler handler, gpointer user_data) {
  gpointer args[3] = { line, handler, user_data };
  gpointer rval = NULL;
  if (set->indexed) {
    TRIANGLE_SET_REINDEX(set);
    rval = geo_set_each_structure_having_common_segment_id_with_line_until(set, line, triangle_set_each_intersector_for_triangle, args);
  } else {
    rval = geo_set_each_structure_until(set, triangle_set_each_intersector_for_triangle, args);
  }
  return rval;
}

VALUE
rb_triangle_set_intersectors(VALUE self, VALUE line) {
  GeoSet *me;
  Line *l;
  VALUE rval;
  CHECK_LINE(line);
  GEO_SET(self, me);
  LINE(line, l);
  rval = rb_ary_new();
  triangle_set_each_intersector_until(me, l, triangle_set_add_to_rb_ary, &rval);
  return rval;
}

static gpointer
triangle_set_intersecting(GeoSet *set, gpointer triangle_gp, gpointer line_gp) {
  Line *line = (Line *) line_gp;
  Triangle *triangle = (Triangle *) triangle_gp;
  if (triangle_intersects(triangle, line))
    return triangle;
  return NULL;
}

static gboolean
triangle_set_intersects(GeoSet *set, Line *line) {
  TRIANGLE_SET_REINDEX(set);
  if (set->indexed) {
    return GBOOL2RB(geo_set_each_structure_having_common_segment_id_with_line_until(set, 
										    line, 
										    triangle_set_intersecting, 
										    line) != NULL);
  } else {
    return GBOOL2RB(geo_set_each_structure_until(set, 
						 triangle_set_intersecting, 
						 line) != NULL);
  }
}

VALUE
rb_triangle_set_intersects(VALUE self, VALUE line) {
  GeoSet *me;
  Line *l;
  CHECK_LINE(line);
  GEO_SET(self, me);
  LINE(line, l);
  return GBOOL2RB(triangle_set_intersects(me, l));
}

static gpointer
triangle_set_find_container(GeoSet *Set, gpointer triangle_gp, gpointer point_gp) {
  Point *point = (Point *) point_gp;
  Triangle *triangle = (Triangle *) triangle_gp;
  if (triangle_contains(triangle, point))
    return triangle;
  return NULL;
}

static Triangle*
triangle_set_first_container(GeoSet *set, Point *point) {
  if (set->indexed) {
    TRIANGLE_SET_REINDEX(set);
    return ( (Triangle *) geo_set_each_structure_having_common_segment_id_with_point_until(set,
											   point,
											   triangle_set_find_container,
											   point));
  } else {
    return ( (Triangle *) geo_set_each_structure_until(set, triangle_set_find_container, point));
  }
}

VALUE
rb_triangle_set_first_container(VALUE self, VALUE p) {
  GeoSet *set;
  Point *point;
  Triangle *rval;
  CHECK_POINT(p);
  GEO_SET(self, set);
  POINT(p, point);
  if ((rval = triangle_set_first_container(set, point)) != NULL)
    return rval->rbTriangle;
  return Qnil;
}

VALUE
rb_triangle_set_contains(VALUE self, VALUE p) {
  GeoSet *set;
  Point *point;
  CHECK_POINT(p);
  GEO_SET(self, set);
  POINT(p, point);
  return GBOOL2RB(triangle_set_first_container(set, point) != NULL);
}

VALUE
rb_triangle_set_clone(VALUE self) {
  GeoSet *me;
  GEO_SET(self, me);
  return RB_TRIANGLE_SET(geo_set_clone(me), CLASS(self));
}

VALUE
rb_triangle_set_structures_in_segment(VALUE self, VALUE x, VALUE y) {
  return rb_geo_set_structures_in_segment(self, x, y, rb_triangle_from_gpointer);
}

/*
 * Segment indexing.
 */

static void
g_hash_table_call_handle_bounds(gpointer key, gpointer value, gpointer user_data) {
  Point bottom_left;
  Point top_right;
  Triangle *t = (Triangle *) key;
  TRIANGLE_BOUNDS(t, &bottom_left, &top_right);
  geo_set_handle_bounds((GeoSet *) user_data, &bottom_left, &top_right);
}

static void
triangle_set_bound_finder(GeoSet *set) {
  g_hash_table_foreach(set->table, g_hash_table_call_handle_bounds, set);
}

static gpointer
triangle_set_insert_segment_id_into_gtree(GeoSet *set, guint32 segment_id, gpointer tree) {
  g_tree_insert((GTree *) tree, GUINT_TO_POINTER(segment_id), GUINT_TO_POINTER(segment_id));
  return NULL;
}

static gboolean
triangle_set_each_segment_id_and_contained(gpointer key, gpointer value, gpointer user_data) {
  gpointer *args = (gpointer *) user_data;
  guint32 segment_id = GPOINTER_TO_UINT(key);
  guint32 tmp_segment_id;
  guint16 i;
  guint16 x = segment_id >> 16;
  guint16 y = (segment_id << 16) >> 16;
  guint16 *last_x = (guint16 *) args[0];
  guint16 *last_y = (guint16 *) args[1];
  geo_set_segment_id_handler handler = (geo_set_segment_id_handler) args[2];
  GeoSet *set = (GeoSet *) args[3];
  gpointer *rval = args[4];
  gpointer handler_args = args[5];
  if (*last_x == x && y - *last_y > 1) {
    for (i = *last_y + 1; i < y; i++) {
      tmp_segment_id = (x << 16) + i;
      if ((*rval = handler(set, tmp_segment_id, handler_args)) != NULL) {
	return TRUE;
      }
    }
  }
  if ((*rval = handler(set, segment_id, handler_args)) != NULL) {
    return TRUE;
  }
  *last_x = x;
  *last_y = y;
  return FALSE;
}

gpointer
geo_set_each_segment_id_of_triangle_until(GeoSet *set, 
					  Triangle *triangle, 
					  geo_set_segment_id_handler handler, 
					  gpointer user_data) {
  Point bottom_left;
  Point top_right;
  GHashTable *seen_segment_ids = g_hash_table_new(g_direct_hash, g_direct_equal);
  gpointer rval = NULL;
  GTree *tree = g_tree_new(gpointer_compare_as_uint);
  gpointer filler_args[4] = { NULL, triangle_set_insert_segment_id_into_gtree, tree, seen_segment_ids };
  Line tmp;
  guint16 last_x = -1;
  guint16 last_y = -1;
  gpointer tree_args[6] = { &last_x, &last_y, handler, set, &rval, user_data };
  TRIANGLE_BOUNDS(triangle, &bottom_left, &top_right);
  /*
   * Put the segment ids of each line in the tree.
   */
  tmp.p1 = triangle->p1;
  tmp.p2 = triangle->p2;
  filler_args[0] = &tmp;
  geo_set_each_segment_line_until(set, &bottom_left, &top_right, geo_set_each_segment_id_of_intersection_until, filler_args);
  tmp.p2 = triangle->p3;
  geo_set_each_segment_line_until(set, &bottom_left, &top_right, geo_set_each_segment_id_of_intersection_until, filler_args);
  tmp.p1 = triangle->p2;
  geo_set_each_segment_line_until(set, &bottom_left, &top_right, geo_set_each_segment_id_of_intersection_until, filler_args);
  /*
   * Put the segment ids of each point in the tree.
   */
  geo_set_each_segment_id_for_point_until(set, triangle->p1, seen_segment_ids, triangle_set_insert_segment_id_into_gtree, tree);
  geo_set_each_segment_id_for_point_until(set, triangle->p2, seen_segment_ids, triangle_set_insert_segment_id_into_gtree, tree);
  geo_set_each_segment_id_for_point_until(set, triangle->p3, seen_segment_ids, triangle_set_insert_segment_id_into_gtree, tree);
  /*
   * Go through the found segment ids (and all contained in them) and call the handler for each one.
   */
  g_tree_foreach(tree, triangle_set_each_segment_id_and_contained, tree_args);
  g_tree_destroy(tree);
  g_hash_table_destroy(seen_segment_ids);
  return rval;
}

gpointer
geo_set_each_structure_having_common_segment_id_with_triangle_until(GeoSet *set, 
								    Triangle *line, 
								    geo_set_structure_handler handler, 
								    gpointer user_data) {
  gpointer rval = NULL;
  GHashTable *seen_structures = g_hash_table_new(g_direct_hash, g_direct_equal);
  gpointer args[3] = { seen_structures, handler, user_data };
  rval = geo_set_each_segment_id_of_triangle_until(set, line, geo_set_each_structure_in_segment_id_until, args);
  g_hash_table_destroy(seen_structures);
  return rval;
}

VALUE
rb_triangle_set_triangles_with_common_segment_id(VALUE self, VALUE l) {
  GeoSet *me;
  Triangle *triangle;
  VALUE rval;
  gpointer args[2];
  CHECK_TRIANGLE(l);
  GEO_SET(self, me);
  TRIANGLE(l, triangle);
  TRIANGLE_SET_REINDEX(me);
  rval = rb_ary_new();
  args[0] = &rval;
  args[1] = rb_triangle_from_gpointer;
  geo_set_each_structure_having_common_segment_id_with_triangle_until(me, triangle, geo_set_insert_structure_into_rb_ary, args);
  return rval;
}

static void
g_hash_table_each_segment_id_for_triangle(gpointer key, gpointer value, gpointer user_data) {
  gpointer *args = (gpointer *) user_data;
  geo_set_each_segment_id_of_triangle_until((GeoSet *) args[0],
						 (Triangle *) key,
						 (geo_set_segment_id_handler) args[1],
						 (Triangle *) key);
}

static void
triangle_set_segment_finder(GeoSet *set) {
  gpointer args[2] = { set, geo_set_insert_into_segment };
  g_hash_table_foreach(set->table, g_hash_table_each_segment_id_for_triangle, args);
}

static gboolean
triangle_set_reindex(GeoSet *set) {
  return geo_set_reindex(set, triangle_set_bound_finder, triangle_set_segment_finder);
}

VALUE
rb_triangle_set_reindex(VALUE self) {
  GeoSet *me;
  GEO_SET(self, me);
  return GBOOL2RB(triangle_set_reindex(me));
}

VALUE
rb_triangle_set_segment_ids_for_triangle(VALUE self, VALUE t) {
  GeoSet *me;
  Triangle *triangle;
  VALUE rval;
  CHECK_TRIANGLE(t);
  GEO_SET(self, me);
  TRIANGLE(t, triangle);
  TRIANGLE_SET_REINDEX(me);
  rval = rb_ary_new();
  geo_set_each_segment_id_of_triangle_until(me, triangle, geo_set_insert_segment_id_into_rb_ary, &rval);
  return rval;
}

static gpointer
triangle_set_each_container(GeoSet *set, gpointer triangle, gpointer user_data) {
  gpointer *args = (gpointer *) user_data;
  Point *point = (Point *) args[0];
  geo_set_structure_handler handler = (geo_set_structure_handler) args[1];
  gpointer handler_args = args[2];
  gpointer rval;
  if (triangle_contains((Triangle *) triangle, (Point *) point)) {
    if ((rval = handler(set, triangle, handler_args)) != NULL)
      return rval;
  }
  return NULL;
}

gpointer
triangle_set_each_container_until(GeoSet *set, Point *p, geo_set_structure_handler handler, gpointer handler_args) {
  gpointer rval = NULL;
  gpointer args[3] = { p, handler, handler_args };
  if (set->indexed) {
    TRIANGLE_SET_REINDEX(set);
    rval = geo_set_each_structure_having_common_segment_id_with_point_until(set,
									    p,
									    triangle_set_each_container,
									    args);
  } else {
    rval = geo_set_each_structure_until(set, triangle_set_each_container, args);
  }
  return rval;
}

static gpointer
geo_set_add_container_to_rb_ary(GeoSet *set, gpointer triangle, gpointer ary) {
  rb_ary_push(*( (VALUE *) ary ), ( (Triangle *) triangle )->rbTriangle);
  return NULL;
}

VALUE
rb_triangle_set_containers(VALUE self, VALUE p) {
  Point *point;
  GeoSet *set;
  VALUE rval;
  CHECK_POINT(p);
  GEO_SET(self, set);
  POINT(p, point);
  rval = rb_ary_new();
  triangle_set_each_container_until(set, point, geo_set_add_container_to_rb_ary, &rval);
  return rval;
}

void
init_triangle_set_o() {
}
