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

VALUE rb_point_set;


static void
g_hash_table_mark_point(gpointer key, gpointer value, gpointer user_data) {
  rb_gc_mark( ( (Point *) key )->rbPoint);
}

void
point_set_mark(GeoSet *set) {
  g_hash_table_foreach(set->table, g_hash_table_mark_point, NULL);
}


VALUE
rb_point_set_alloc(VALUE class) {
  GeoSet *set = new_geo_set();
  return RB_POINT_SET(set, class);
}

VALUE
rb_point_set_insert(VALUE self, VALUE p) {
  Point *point;
  GeoSet *me;
  CHECK_POINT(p);
  GEO_SET(self, me);
  POINT(p, point);
  geo_set_insert(me, (gpointer ) point);
  return self;
}

VALUE
rb_point_set_include(VALUE self, VALUE point) {
  GeoSet *me;
  Point *t;
  CHECK_POINT(point);
  POINT(point, t);
  GEO_SET(self, me);
  return GBOOL2RB(g_hash_table_lookup_extended(me->table, (gpointer) t, NULL, NULL));
}

VALUE
rb_point_set_delete(VALUE self, VALUE point) {
  GeoSet *me;
  Point *t;
  CHECK_POINT(point);
  GEO_SET(self, me);
  POINT(point, t);
  return geo_set_delete(me, (gpointer) t, point);
}

static gpointer
g_hash_table_yield_point(GeoSet *set, gpointer structure, gpointer user_data) {
  rb_yield(( (Point *) structure )->rbPoint);
  return NULL;
}

VALUE
rb_point_set_each(VALUE self) {
  GeoSet *set;
  GEO_SET(self, set);
  geo_set_each_structure_until(set, g_hash_table_yield_point, NULL);
  return self;
}

VALUE
rb_point_set_clone(VALUE self) {
  GeoSet *me;
  GEO_SET(self, me);
  return RB_POINT_SET(geo_set_clone(me), CLASS(self));
}

gpointer
geo_set_each_structure_having_common_segment_id_with_point_until(GeoSet *set, 
								 Point *point, 
								 geo_set_structure_handler handler, 
								 gpointer user_data) {
  GHashTable *seen_structures = g_hash_table_new(g_direct_hash, g_direct_equal);
  gpointer args[3] = { seen_structures, handler, user_data };
  GHashTable *seen_segment_ids = g_hash_table_new(g_direct_hash, g_direct_equal);
  gpointer rval = NULL;
  rval = geo_set_each_segment_id_for_point_until(set, 
						 point, 
						 seen_segment_ids, 
						 geo_set_each_structure_in_segment_id_until, 
						 args);
  g_hash_table_destroy(seen_segment_ids);
  g_hash_table_destroy(seen_structures);
  return rval;
}

void
init_point_set_o() {
}
