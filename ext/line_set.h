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


#ifndef LINE_SET_H
#define LINE_SET_H

#include <common.h>

extern VALUE rb_line_set;

#define RB_LINE_SET(line_set_pointer,klass) ((line_set_pointer)->rbGeoSet = Data_Wrap_Struct(klass, line_set_mark, geo_set_free, (line_set_pointer)))
#define LINE_SET_REINDEX(set) if (set->index_dirty && set->indexed) line_set_reindex(set)
#define LINE_SET_P(l) (!NIL_P((l)) && rb_is_a((l), rb_line_set))

typedef gboolean
(*line_set_line_handler)(GeoSet *set, Line *line, gpointer user_data);

void
line_set_mark(GeoSet *set);

VALUE
rb_line_set_alloc(VALUE class);

VALUE
rb_line_set_insert(VALUE self, VALUE line);

VALUE
rb_line_set_include(VALUE self, VALUE line);

VALUE
rb_line_set_delete(VALUE self, VALUE line);

VALUE
rb_line_set_each(VALUE self);

VALUE
rb_line_set_intersects(VALUE self, VALUE l);

VALUE
rb_line_set_slide(VALUE self, VALUE l);

VALUE
rb_line_set_intersections(VALUE self, VALUE l);

VALUE
rb_line_set_intersection_free_endpoints(int argc, VALUE *argv, VALUE self);

VALUE
rb_line_set_closest_intersection(VALUE self, VALUE l);

VALUE
rb_line_set_clone(VALUE self);

VALUE
rb_line_set_n_intersections(VALUE self, VALUE line);

VALUE 
rb_line_set_reindex(VALUE self);

void
g_hash_table_clone_line_value_into_rb_value(gpointer key, gpointer value, gpointer user_data);

VALUE
rb_line_set_segment_lines(VALUE self, VALUE line);

VALUE
rb_line_set_structures_in_segment(VALUE self, VALUE x, VALUE y);

VALUE
rb_line_set_segment_ids_for_line(VALUE self, VALUE l);

VALUE
rb_line_set_lines_with_common_segment_id(VALUE self, VALUE l);

gpointer
geo_set_each_segment_id_of_intersection_until(GeoSet *set, Line *segment_line, gpointer user_data);

gpointer
geo_set_each_segment_id_of_line_until(GeoSet *set, Line *line, geo_set_segment_id_handler handler, gpointer user_data);

gpointer
geo_set_each_structure_having_common_segment_id_with_line_until(GeoSet *set, 
								Line *line, 
								geo_set_structure_handler handler, 
								gpointer user_data);

void
line_set_slide_point(GeoSet *set, Line *line, gint ttl);

void
line_set_each_intersection_free_endpoint(GeoSet *set, 
					 GeoSet *origins, 
					 GeoSet *endpoints, 
					 gdouble max_distance, 
					 geo_set_structure_handler handler,
					 gpointer user_data);

#endif
