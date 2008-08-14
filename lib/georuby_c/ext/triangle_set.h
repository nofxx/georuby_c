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


#ifndef TRIANGLE_SET_H
#define TRIANGLE_SET_H

#include <common.h>

extern VALUE rb_triangle_set;


#define RB_TRIANGLE_SET(triangle_set_pointer,klass) ((triangle_set_pointer)->rbGeoSet = Data_Wrap_Struct(klass, triangle_set_mark, geo_set_free, (triangle_set_pointer)))
#define TRIANGLE_SET_REINDEX(set) if (set->index_dirty && set->indexed) triangle_set_reindex(set)
#define TRIANGLE_SET_P(l) (!NIL_P((l)) && rb_is_a((l), rb_triangle_set))

VALUE
rb_triangle_set_alloc(VALUE class);

VALUE
rb_triangle_set_insert(VALUE self, VALUE t);

VALUE
rb_triangle_set_include(VALUE self, VALUE triangle);

VALUE
rb_triangle_set_delete(VALUE self, VALUE triangle);

VALUE
rb_triangle_set_each(VALUE self);

VALUE
rb_triangle_set_intersectors(VALUE self, VALUE line);

VALUE
rb_triangle_set_intersects(VALUE self, VALUE line);

VALUE
rb_triangle_set_first_container(VALUE self, VALUE p);

VALUE
rb_triangle_set_containers(VALUE self, VALUE p);

gpointer
triangle_set_each_container_until(GeoSet *set, Point *p, geo_set_structure_handler handler, gpointer handler_args);

VALUE
rb_triangle_set_contains(VALUE self, VALUE p);

VALUE
rb_triangle_set_clone(VALUE self);

VALUE
rb_triangle_set_reindex(VALUE self);

VALUE
rb_triangle_set_segment_ids_for_triangle(VALUE self, VALUE t);

VALUE
rb_triangle_set_triangles_with_common_segment_id(VALUE self, VALUE l);

VALUE
rb_triangle_set_structures_in_segment(VALUE self, VALUE x, VALUE y);

gpointer
geo_set_each_segment_id_of_triangle_until(GeoSet *set, 
					  Triangle *triangle, 
					  geo_set_segment_id_handler handler, 
					  gpointer user_data);

gpointer
geo_set_each_structure_having_common_segment_id_with_triangle_until(GeoSet *set, 
								    Triangle *line, 
								    geo_set_structure_handler handler, 
								    gpointer user_data);

void
triangle_set_mark(GeoSet *set);

gpointer
triangle_set_each_intersector_until(GeoSet *set, Line *line, geo_set_structure_handler handler, gpointer user_data);

#endif
