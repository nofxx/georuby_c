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


#ifndef GEO_SET_H
#define GEO_SET_H

#include <common.h>

struct IntersectionStruct;
enum IntersectionTypeEnum;

#define GEO_SET(rb_set,geo_set_pointer) Data_Get_Struct((rb_set), GeoSet, (geo_set_pointer))
#define GEO_SET_WIDTH(table) ((table)->top_right->x - (table)->bottom_left->x)
#define GEO_SET_HEIGHT(table) ((table)->top_right->y - (table)->bottom_left->y)
#define GEO_SET_HORIZONTAL_SEGMENTS(table) ((gint) ceil(GEO_SET_WIDTH((table)) / (table)->segment_side))
#define GEO_SET_VERTICAL_SEGMENTS(table) ((gint) ceil(GEO_SET_HEIGHT((table)) / (table)->segment_side)) 
#define CHECK_ITERATING(geo_set) if ((geo_set)->iterating) rb_raise(rb_eRuntimeError, "You can not modify this instance while iterating over it!")

#define MAX_GEO_SET_SEGMENTS G_MAXUINT16

/*
 * Should call geo_set_handle_bounds for each contained element with the bounds of
 * the element as arguments.
 */
typedef void
(*geo_set_bound_finder)(GeoSet *set);

/*
 * Should call geo_set_insert_under_segment_ids_for_point for each
 * intersection between each contained element and the segment lines
 * of the given set.
 *
 * Use geo_set_each_segment_line to walk through the segment lines
 * relevant for any contained element.
 */
typedef void
(*geo_set_segment_finder)(GeoSet *set);

/*
 * Should return the ruby VALUE containing the given geo structure.
 */
typedef VALUE
(*geo_structure_to_value)(gpointer structure);

typedef gpointer
(*geo_set_segment_id_handler)(GeoSet *set, guint32 segment_id, gpointer user_data);

typedef gpointer
(*geo_set_structure_handler)(GeoSet *set, gpointer structure_in_id, gpointer user_data);

typedef gpointer
(*geo_set_segment_line_handler)(GeoSet *set, Line *line, gpointer user_data);

void
geo_set_free(GeoSet *t);

VALUE
rb_geo_set_inspect(VALUE self);

VALUE
rb_geo_set_size(VALUE self);

GeoSet*
new_geo_set();

void
geo_set_insert(GeoSet *set, gpointer content);

GeoSet*
geo_set_clone(GeoSet *set);

gboolean
geo_set_reindex(GeoSet *set, geo_set_bound_finder bound_finder, geo_set_segment_finder segment_finder);

VALUE
geo_set_delete(GeoSet *set, gpointer p, VALUE rval);

VALUE
rb_geo_set_segment_side(VALUE self);

/*
 * Will call handler with set, segment_id and user_data for all segment ids in set that
 * point is a member of until handler returns non NULL, in which case it will stop and
 * return what the handler returned. Will not call handler with segment ids in 
 * seen_segment_ids, and will put all segment ids that handler is called for in 
 * seen_segment_ids.
 */
gpointer
geo_set_each_segment_id_for_point_until(GeoSet *set, 
					Point *point, 
					GHashTable *seen_segment_ids,
					geo_set_segment_id_handler handler, 
					gpointer user_data);

void
geo_set_handle_bounds(GeoSet *set, Point *bottom_left, Point *top_right);

/*
 * Will call handler with segment line and user_data for each segment line between bottom_left and top_right
 * until handler returns non NULL, in which case it will stop and return what the handler returned.
 */
gpointer
geo_set_each_segment_line_until(GeoSet *set, 
				Point *bottom_left, 
				Point *top_right, 
				geo_set_segment_line_handler handler, 
				gpointer user_data);

VALUE
rb_geo_set_structures_in_segment(VALUE self, VALUE x, VALUE y, geo_structure_to_value converter);

gpointer
geo_set_insert_into_segment(GeoSet *set, guint32 segment_id, gpointer geo_struct);

gpointer
geo_set_insert_segment_id_into_rb_ary(GeoSet *set, guint32 segment_id, gpointer ary_ptr);

VALUE
rb_geo_set_segment_lines(VALUE self);

VALUE
rb_geo_set_bottom_left(VALUE self);

VALUE
rb_geo_set_top_right(VALUE self);

VALUE
rb_geo_set_width(VALUE self);

VALUE
rb_geo_set_height(VALUE self);

/*
 * To enable calling this function from functions requiring geo_set_segment_id_handlers
 * we have this signature.
 * 
 * meta is supposed to look like { GHashTable *seen_structures, geo_set_structure_handler handler, gpointer handler_args }
 *
 * Will call the handler with set, structure and user_data for each
 * structure in segment with segment_id in set, until handler returns
 * non NULL - in which case it will stop and return what the handler returned.
 * Will not call handler with structures in seen_structures, and
 * will put all structures that handler is called for in seen_structures.
 */
gpointer
geo_set_each_structure_in_segment_id_until(GeoSet *set, guint32 segment_id, gpointer meta);

gpointer
geo_set_insert_structure_into_rb_ary(GeoSet *set, gpointer structure, gpointer user_data);

VALUE
rb_geo_set_set_indexed(VALUE self, VALUE new_indexed);

VALUE
rb_geo_set_get_indexed(VALUE self);

gpointer
geo_set_each_structure_until(GeoSet *set, geo_set_structure_handler handler, gpointer handler_args);

#endif
