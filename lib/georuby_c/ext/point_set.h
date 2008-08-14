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


#ifndef POINT_SET_H
#define POINT_SET_H

#include <common.h>

extern VALUE rb_point_set;

#define RB_POINT_SET(point_set_pointer,klass) ((point_set_pointer)->rbGeoSet = Data_Wrap_Struct(klass, point_set_mark, geo_set_free, (point_set_pointer)))

#define POINT_SET_P(pl) (!NIL_P((pl)) && rb_is_a((pl), rb_point_set))
#define CHECK_POINT_SET(pl) if (!POINT_SET_P((pl))) rb_raise(rb_eTypeError, "Expected Geo::PointSet!")

void
point_set_mark(GeoSet *set);

VALUE
rb_point_set_alloc(VALUE class);

VALUE
rb_point_set_insert(VALUE self, VALUE p);

VALUE
rb_point_set_include(VALUE self, VALUE point);

VALUE
rb_point_set_delete(VALUE self, VALUE point);

VALUE
rb_point_set_each(VALUE self);

VALUE
rb_point_set_clone(VALUE self);

gpointer
geo_set_each_structure_having_common_segment_id_with_point_until(GeoSet *set, 
								 Point *point, 
								 geo_set_structure_handler handler, 
								 gpointer user_data);




#endif
