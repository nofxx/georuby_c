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

VALUE
rb_geo_set_inspect(VALUE self) {
  gchar rval[1024];
  GeoSet *l;
  GEO_SET(self, l);
  snprintf(rval, 1024, "<%s:%p size=%u>", rb_obj_classname(self), l, g_hash_table_size(l->table));
  return rb_str_new2(rval);
}

VALUE
rb_geo_set_size(VALUE self) {
  GeoSet *set;
  GEO_SET(self, set);
  return INT2NUM(g_hash_table_size(set->table));
}

static void
g_hash_table_destroy_notify(gpointer p) {
  g_hash_table_destroy((GHashTable *) p);
}

void
geo_set_free(GeoSet *t) {
  free(t->bottom_left);
  free(t->top_right);
  g_hash_table_destroy(t->table);
  g_hash_table_destroy(t->segments);
  g_hash_table_destroy(t->segment_lines);
  free(t);
}

GeoSet*
new_geo_set() {
  GeoSet *rval = ALLOC(GeoSet);
  rval->iterating = FALSE;
  rval->bottom_left = new_point(0,0);
  rval->top_right = new_point(0,0);
  rval->segment_side = 0;
  rval->segments = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, g_hash_table_destroy_notify);
  rval->segment_lines = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, line_destroy_notify);
  rval->index_dirty = TRUE;
  rval->indexed = TRUE;
  rval->table = g_hash_table_new(g_direct_hash, g_direct_equal);
  rval->rbGeoSet = 0;
  return rval;
}

void
geo_set_insert(GeoSet *set, gpointer content) {
  CHECK_ITERATING(set);
  g_hash_table_insert(set->table, content, content);
  set->index_dirty = TRUE;
}


static void
g_hash_table_insert_into_other(gpointer key, gpointer value, gpointer user_data) {
  g_hash_table_insert((GHashTable *) user_data, key, value);
}

static void
g_hash_table_insert_line_copy_into_other(gpointer key, gpointer value, gpointer user_data) {
  Line *l = (Line *) value;
  Line *copy = new_line_with_coordinates(l->p1->x, l->p1->y, l->p2->x, l->p2->y);
  g_hash_table_insert((GHashTable *) user_data, key, copy);
}

static void
g_hash_table_insert_segments_into_other(gpointer key, gpointer value, gpointer user_data) {
  GHashTable *target_segments = (GHashTable *) user_data;
  GHashTable *source_segment = (GHashTable *) value;
  GHashTable *target_segment = g_hash_table_new(g_direct_hash, g_direct_equal);
  g_hash_table_foreach(source_segment, g_hash_table_insert_into_other, target_segment);
  g_hash_table_insert(target_segments, key, target_segment);
}

GeoSet*
geo_set_clone(GeoSet *set) {
  GeoSet *rval = new_geo_set();
  g_hash_table_foreach(set->table, g_hash_table_insert_into_other, rval->table);
  g_hash_table_foreach(set->segments, g_hash_table_insert_segments_into_other, rval->segments);
  g_hash_table_foreach(set->segment_lines, g_hash_table_insert_line_copy_into_other, rval->segment_lines);
  return rval;
}

void
geo_set_handle_bounds(GeoSet *set, Point *bottom_left, Point *top_right) {
  if (set->bottom_left == NULL) {
    set->bottom_left = new_point(bottom_left->x, bottom_left->y);
  } else {
    if (set->bottom_left->x > bottom_left->x)
      set->bottom_left->x = bottom_left->x;
    if (set->bottom_left->y > bottom_left->y)
      set->bottom_left->y = bottom_left->y;
  }
  if (set->top_right == NULL) {
    set->top_right = new_point(top_right->x, top_right->y);
  } else {
    if (set->top_right->x < top_right->x)
      set->top_right->x = top_right->x;
    if (set->top_right->y < top_right->y)
      set->top_right->y = top_right->y;
  }
}

static void
geo_set_find_bounds(GeoSet *set, geo_set_bound_finder bound_finder) {
  free(set->bottom_left);
  free(set->top_right);
  set->bottom_left = NULL;
  set->top_right = NULL;
  bound_finder(set);
  set->segment_side = sqrt(MAX(1, GEO_SET_WIDTH(set)) * MAX(1, GEO_SET_HEIGHT(set)) / g_hash_table_size(set->table));
  if (GEO_SET_HORIZONTAL_SEGMENTS(set) > MAX_GEO_SET_SEGMENTS)
    set->segment_side = MAX(1, GEO_SET_WIDTH(set)) / MAX_GEO_SET_SEGMENTS;
  if (GEO_SET_VERTICAL_SEGMENTS(set) > MAX_GEO_SET_SEGMENTS)
    set->segment_side = MAX(1, GEO_SET_HEIGHT(set)) / MAX_GEO_SET_SEGMENTS;
}

static void
geo_set_create_segment_lines(GeoSet *set) {
  guint32 tmp;
  g_hash_table_destroy(set->segment_lines);
  set->segment_lines = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, line_destroy_notify);
  for (tmp = 0; set->bottom_left->x + (tmp * set->segment_side) < set->top_right->x + set->segment_side; tmp ++) {
    g_hash_table_insert(set->segment_lines, 
			GUINT_TO_POINTER((tmp << 16) + G_MAXUINT16), 
			new_line_with_coordinates(set->bottom_left->x + tmp * set->segment_side, 
						  set->bottom_left->y, 
						  set->bottom_left->x + tmp * set->segment_side, 
						  set->top_right->y));
  }
  for (tmp = 0; set->bottom_left->y + (tmp * set->segment_side) < set->top_right->y + set->segment_side; tmp++) {
    g_hash_table_insert(set->segment_lines,
			GUINT_TO_POINTER(tmp),
			new_line_with_coordinates(set->bottom_left->x,
						  set->bottom_left->y + tmp * set->segment_side,
						  set->top_right->x,
						  set->bottom_left->y + tmp * set->segment_side));
  }
}

VALUE
rb_geo_set_structures_in_segment(VALUE self, VALUE xV, VALUE yV, geo_structure_to_value converter) {
  guint32 segment_id;
  GeoSet *me;
  VALUE rval;
  gpointer args[3];
  gpointer inner_args[2];
  guint16 x;
  guint16 y;
  GHashTable *seen_structures = g_hash_table_new(g_direct_hash, g_direct_equal);
  CHECK_NUMERICALITY(xV);
  CHECK_NUMERICALITY(yV);
  GEO_SET(self, me);
  x = (guint16) NUM2INT(xV);
  y = (guint16) NUM2INT(yV);
  segment_id = (x << 16) + y;
  rval = rb_ary_new();
  inner_args[0] = &rval;
  inner_args[1] = converter;
  args[0] = seen_structures;
  args[1] = geo_set_insert_structure_into_rb_ary;
  args[2] = inner_args;
  geo_set_each_structure_in_segment_id_until(me, segment_id, args);
  g_hash_table_destroy(seen_structures);
  return rval;
}

static gboolean
g_hash_table_call_handler_for_unique_structure_until(gpointer key, gpointer value, gpointer user_data) {
  gpointer *args = (gpointer *) user_data;
  GeoSet *set = (GeoSet *) args[0];
  geo_set_structure_handler handler = (geo_set_structure_handler) args[1];
  GHashTable *seen_structures = (GHashTable *) args[2];
  gpointer *rval = (gpointer *) args[3];
  if (g_hash_table_lookup(seen_structures, key) == NULL) {
    g_hash_table_insert(seen_structures, key, GUINT_TO_POINTER(1));
    if ((*rval = handler(set, key, args[4])) != NULL) {
      return TRUE;
    } else {
      return FALSE;
    }
  }
  return FALSE;
}

/*
 * To enable calling this function from functions requiring geo_set_segment_id_handlers
 * we have this signature.
 * 
 * meta is supposed to look like { GHashTable *seen_structures, geo_set_structure_handler handler, gpointer handler_args }
 */
gpointer
geo_set_each_structure_in_segment_id_until(GeoSet *set, guint32 segment_id, gpointer meta) {
  gpointer *args = (gpointer *) meta;
  GHashTable *seen_structures = (GHashTable *) args[0];
  geo_set_structure_handler handler = (geo_set_structure_handler) args[1];
  gpointer handler_args = args[2];
  GHashTable *structures_in_id = (GHashTable *) g_hash_table_lookup(set->segments, GUINT_TO_POINTER(segment_id));
  if (structures_in_id != NULL) {
    gpointer rval = NULL;
    gpointer args[5] = { set, handler, seen_structures, &rval, handler_args };
    g_hash_table_find(structures_in_id, g_hash_table_call_handler_for_unique_structure_until, args);
    return rval;
  } else {
    return NULL;
  }
}

gpointer
geo_set_insert_into_segment(GeoSet *set, guint32 segment_id, gpointer geo_struct) {
  GHashTable *segment = (GHashTable *) g_hash_table_lookup(set->segments, GUINT_TO_POINTER(segment_id));
  if (segment == NULL) {
    segment = g_hash_table_new(g_direct_hash, g_direct_equal);
    g_hash_table_insert(set->segments, GUINT_TO_POINTER(segment_id), segment);
  }
  g_hash_table_insert(segment, geo_struct, geo_struct);
  return NULL;
}

gpointer
geo_set_insert_segment_id_into_rb_ary(GeoSet *set, guint32 segment_id, gpointer ary_ptr) {
  rb_ary_push(*( (VALUE *) ary_ptr), rb_ary_new3(2, 
						 INT2NUM((guint16) (segment_id >> 16)), 
						 INT2NUM((guint16) ((segment_id << 16) >> 16))));
  return NULL;
}

gpointer
geo_set_insert_structure_into_rb_ary(GeoSet *set, gpointer structure, gpointer user_data) {
  gpointer *args = (gpointer *) user_data;
  rb_ary_push(*( (VALUE *) args[0] ), ( (geo_structure_to_value) args[1] )(structure));
  return NULL;
}

static gpointer
call_geo_set_segment_handler_if_unseen_id(geo_set_segment_id_handler handler, 
					  GeoSet *set, 
					  gpointer user_data, 
					  GHashTable *seen_segment_ids,
					  guint16 x,
					  guint16 y) {
  guint32 segment_id = (x << 16) + y;
  if (g_hash_table_lookup(seen_segment_ids, GUINT_TO_POINTER(segment_id)) == NULL) {
    gpointer rval = NULL;
    rval = handler(set, segment_id, user_data);
    g_hash_table_insert(seen_segment_ids, GUINT_TO_POINTER(segment_id), GUINT_TO_POINTER(1));
    return rval;
  }
  return NULL;
}

gpointer
geo_set_each_segment_id_for_point_until(GeoSet *set, 
					Point *point, 
					GHashTable *seen_segment_ids,
					geo_set_segment_id_handler handler, 
					gpointer user_data) {
  if (point->x < set->bottom_left->x || 
      point->y < set->bottom_left->y || 
      point->x > set->top_right->x + set->segment_side || 
      point->y > set->top_right->y + set->segment_side)
    return NULL;
  gdouble horizontal_segment_quota = (point->x - set->bottom_left->x) / set->segment_side;
  gdouble vertical_segment_quota = (point->y - set->bottom_left->y) / set->segment_side;
  gpointer rval = NULL;
  if (IS_INTLIKE(vertical_segment_quota)) {
    guint16 rounded_vertical = (guint16) round(vertical_segment_quota);
    if (IS_INTLIKE(horizontal_segment_quota)) {
      guint16 rounded_horizontal = (guint16) round(horizontal_segment_quota);
      if (rounded_horizontal > 0) {
	if (rounded_vertical > 0) {
	  if ((rval = call_geo_set_segment_handler_if_unseen_id(handler, 
								set, 
								user_data, 
								seen_segment_ids, 
								rounded_horizontal - 1, 
								rounded_vertical - 1)) != NULL)
	    return rval;
	}
	if ((rval = call_geo_set_segment_handler_if_unseen_id(handler, 
							      set, 
							      user_data, 
							      seen_segment_ids,
							      rounded_horizontal - 1,
							      rounded_vertical)) != NULL)
	  return rval;
      }
      if (rounded_vertical > 0) {
	if ((rval = call_geo_set_segment_handler_if_unseen_id(handler, 
							      set, 
							      user_data, 
							      seen_segment_ids,
							      rounded_horizontal,
							      rounded_vertical - 1)) != NULL)
	  return rval;
      }
      if ((rval = call_geo_set_segment_handler_if_unseen_id(handler, 
							    set, 
							    user_data, 
							    seen_segment_ids,
							    rounded_horizontal,
							    rounded_vertical)) != NULL)
	return rval;
    } else {
      if (rounded_vertical > 0) {
	if ((rval = call_geo_set_segment_handler_if_unseen_id(handler, 
							      set, 
							      user_data, 
							      seen_segment_ids,
							      (guint16) horizontal_segment_quota,
							      rounded_vertical - 1)) != NULL)
	  return rval;
      }
      if ((rval = call_geo_set_segment_handler_if_unseen_id(handler, 
							    set, 
							    user_data, 
							    seen_segment_ids,
							    (guint16) horizontal_segment_quota,
							    rounded_vertical)) != NULL)
	return rval;
    }
  } else { 
    if (IS_INTLIKE(horizontal_segment_quota)) {
      guint16 rounded_horizontal = (gint) round(horizontal_segment_quota);
      if (rounded_horizontal > 0) {
	if ((rval = call_geo_set_segment_handler_if_unseen_id(handler, 
							      set, 
							      user_data, 
							      seen_segment_ids,
							      rounded_horizontal - 1,
							      (guint16) vertical_segment_quota)) != NULL)
	  return rval;
      }
      if ((rval = call_geo_set_segment_handler_if_unseen_id(handler, 
							    set, 
							    user_data, 
							    seen_segment_ids,
							    rounded_horizontal,
							    (guint16) vertical_segment_quota)) != NULL)
	return rval;
    } else {
      if ((rval = call_geo_set_segment_handler_if_unseen_id(handler,
							    set, 
							    user_data, 
							    seen_segment_ids,
							    (guint16) horizontal_segment_quota,
							    (guint16) vertical_segment_quota)) != NULL)
	return rval;
    }
  }
  return NULL;
}

gpointer
geo_set_each_segment_line_until(GeoSet *set, 
				Point *bottom_left, 
				Point *top_right, 
				geo_set_segment_line_handler handler, 
				gpointer user_data) {
  gpointer rval = NULL;
  guint16 tmp;
  Line *tmp_line;
  gint left_n = MAX((gint) ceil((bottom_left->x - set->bottom_left->x) / set->segment_side), 
		    0);
  gint bottom_n = MAX((gint) ceil((bottom_left->y - set->bottom_left->y) / set->segment_side),
		      0);
  gint right_n = MIN((gint) ((top_right->x - set->bottom_left->x) / set->segment_side),
		     GEO_SET_HORIZONTAL_SEGMENTS(set));
  gint top_n = MIN((gint) ((top_right->y - set->bottom_left->y) / set->segment_side),
		   GEO_SET_VERTICAL_SEGMENTS(set));
  for (tmp = left_n; tmp <= right_n; tmp++) {
    tmp_line = (Line *) g_hash_table_lookup(set->segment_lines, GUINT_TO_POINTER((guint32) ((tmp << 16) + G_MAXUINT16)));
    if (tmp_line != NULL) {
      if ((rval = handler(set, tmp_line, user_data)) != NULL)
	return rval;
    }
  }
  for (tmp = bottom_n; tmp <= top_n; tmp++) {
    tmp_line = (Line *) g_hash_table_lookup(set->segment_lines, GUINT_TO_POINTER((guint32) tmp));
    if (tmp_line != NULL) {
      if ((rval = handler(set, tmp_line, user_data)) != NULL)
	return rval;
    }
  }
  return NULL;
}

static void
geo_set_find_segments(GeoSet *set, geo_set_segment_finder segment_finder) {
  g_hash_table_destroy(set->segments);
  set->segments = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, g_hash_table_destroy_notify);
  segment_finder(set);
}

gboolean
geo_set_reindex(GeoSet *set, geo_set_bound_finder bound_finder, geo_set_segment_finder segment_finder) {
  if (set->indexed && set->index_dirty && g_hash_table_size(set->table) > 0) {
    set->index_dirty = FALSE;
    geo_set_find_bounds(set, bound_finder);
    if (set->segment_side > 0) {
      geo_set_create_segment_lines(set);
      geo_set_find_segments(set, segment_finder);
      return TRUE;
    } else {
      return FALSE;
    }
  } else {
    return FALSE;
  }
}

VALUE
rb_geo_set_set_indexed(VALUE self, VALUE new_indexed) {
  GeoSet *set;
  GEO_SET(self, set);
  if (new_indexed == Qtrue)
    set->indexed = TRUE;
  else 
    set->indexed = FALSE;
  return GBOOL2RB(set->indexed);
}

VALUE
rb_geo_set_get_indexed(VALUE self) {
  GeoSet *set;
  GEO_SET(self, set);
  return GBOOL2RB(set->indexed);
}

static gboolean
g_hash_table_call_handler_for_structure_until(gpointer key, gpointer value, gpointer user_data) {
  gpointer *args = (gpointer *) user_data;
  GeoSet *set = (GeoSet *) args[0];
  geo_set_structure_handler handler = (geo_set_structure_handler) args[1];
  gpointer *rval = (gpointer *) args[2];
  if ((*rval = handler(set, key, args[3])) != NULL) {
    return TRUE;
  } else {
    return FALSE;
  }
  return FALSE;
}

gpointer
geo_set_each_structure_until(GeoSet *set, geo_set_structure_handler handler, gpointer handler_args) {
  gpointer rval;
  gpointer args[5] = { set, handler, &rval, handler_args };
  g_hash_table_find(set->table, g_hash_table_call_handler_for_structure_until, args);
  return rval;
} 

VALUE
geo_set_delete(GeoSet *set, gpointer p, VALUE rval) {
  CHECK_ITERATING(set);
  if (g_hash_table_remove(set->table, p)) {
    set->index_dirty = TRUE;
    return rval;
  } else {
    return Qnil;
  }
}

VALUE
rb_geo_set_segment_side(VALUE self) {
  GeoSet *set;
  GEO_SET(self, set);
  return rb_float_new(set->segment_side);
}

VALUE
rb_geo_set_segment_lines(VALUE self) {
  GeoSet *set;
  VALUE rval = rb_ary_new();
  GEO_SET(self, set);
  g_hash_table_foreach(set->segment_lines, g_hash_table_clone_line_value_into_rb_value, (gpointer) &rval);
  return rval;
}

VALUE
rb_geo_set_bottom_left(VALUE self) {
  GeoSet *set;
  Point *point;
  GEO_SET(self, set);
  point = new_point(set->bottom_left->x, set->bottom_left->y);
  return RB_POINT(point, rb_point);
}

VALUE
rb_geo_set_top_right(VALUE self) {
  GeoSet *set;
  Point *point;
  GEO_SET(self, set);
  point = new_point(set->top_right->x, set->top_right->y);
  return RB_POINT(point, rb_point);
}

VALUE
rb_geo_set_width(VALUE self) {
  GeoSet *set;
  GEO_SET(self, set);
  return rb_float_new(GEO_SET_WIDTH(set));
}

VALUE
rb_geo_set_height(VALUE self) {
  GeoSet *set;
  GEO_SET(self, set);
  return rb_float_new(GEO_SET_HEIGHT(set));
}
