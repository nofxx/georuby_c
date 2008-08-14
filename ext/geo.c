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
rb_float_intlike(VALUE self) {
  if (IS_INTLIKE(NUM2DBL(self)))
    return Qtrue;
  else
    return Qfalse;
}

#ifdef __cplusplus
extern "C" {
#endif
  void Init_geo() {
    rb_define_method(rb_cFloat, "intlike?", rb_float_intlike, 0);

    VALUE rb_geo = rb_define_module("Geo");
    rb_define_const(rb_geo, "VERSION", rb_str_new2("0.1.6"));

    rb_triangle_set = rb_define_class_under(rb_geo,
					    "TriangleSet",
					    rb_cObject);
    rb_triangle = rb_define_class_under(rb_geo,
					"Triangle",
					rb_cObject);
    rb_line = rb_define_class_under(rb_geo,
				    "Line",
				    rb_cObject);
    rb_point = rb_define_class_under(rb_geo,
				     "Point",
				     rb_cObject);
    rb_line_set = rb_define_class_under(rb_geo,
					  "LineSet",
					  rb_cObject);
    rb_point_set = rb_define_class_under(rb_geo,
					  "PointSet",
					  rb_cObject);

    rb_define_alloc_func(rb_triangle_set, rb_triangle_set_alloc);
    rb_define_method(rb_triangle_set, "<<", rb_triangle_set_insert, 1);
    rb_define_method(rb_triangle_set, "each", rb_triangle_set_each, 0);
    rb_define_method(rb_triangle_set, "size", rb_geo_set_size, 0);
    rb_define_method(rb_triangle_set, "inspect", rb_geo_set_inspect, 0);
    rb_define_method(rb_triangle_set, "include?", rb_triangle_set_include, 1);
    rb_define_method(rb_triangle_set, "delete!", rb_triangle_set_delete, 1);
    rb_define_method(rb_triangle_set, "contains?", rb_triangle_set_contains, 1);
    rb_define_method(rb_triangle_set, "first_container", rb_triangle_set_first_container, 1);
    rb_define_method(rb_triangle_set, "intersectors", rb_triangle_set_intersectors, 1);
    rb_define_method(rb_triangle_set, "intersects?", rb_triangle_set_intersects, 1);
    rb_define_method(rb_triangle_set, "clone", rb_triangle_set_clone, 0);
    rb_define_method(rb_triangle_set, "_segment_side", rb_geo_set_segment_side, 0);
    rb_define_method(rb_triangle_set, "_segment_lines", rb_geo_set_segment_lines, 0);
    rb_define_method(rb_triangle_set, "_bottom_left", rb_geo_set_bottom_left, 0);
    rb_define_method(rb_triangle_set, "_top_right", rb_geo_set_top_right, 0);
    rb_define_method(rb_triangle_set, "_width", rb_geo_set_width, 0);
    rb_define_method(rb_triangle_set, "_height", rb_geo_set_height, 0);
    rb_define_method(rb_triangle_set, "_reindex", rb_triangle_set_reindex, 0);
    rb_define_method(rb_triangle_set, "_segments_for", rb_triangle_set_segment_ids_for_triangle, 1);
    rb_define_method(rb_triangle_set, "_with_common_segment", rb_triangle_set_triangles_with_common_segment_id, 1);
    rb_define_method(rb_triangle_set, "_in_segment", rb_triangle_set_structures_in_segment, 2);
    rb_define_method(rb_triangle_set, "indexed", rb_geo_set_get_indexed, 0);
    rb_define_method(rb_triangle_set, "indexed=", rb_geo_set_set_indexed, 1);
    rb_define_method(rb_triangle_set, "containers", rb_triangle_set_containers, 1);

    rb_define_alloc_func(rb_point_set, rb_point_set_alloc);
    rb_define_method(rb_point_set, "<<", rb_point_set_insert, 1);
    rb_define_method(rb_point_set, "each", rb_point_set_each, 0);
    rb_define_method(rb_point_set, "size", rb_geo_set_size, 0);
    rb_define_method(rb_point_set, "inspect", rb_geo_set_inspect, 0);
    rb_define_method(rb_point_set, "include?", rb_point_set_include, 1);
    rb_define_method(rb_point_set, "delete!", rb_point_set_delete, 1);
    rb_define_method(rb_point_set, "clone", rb_point_set_clone, 0);

    rb_define_alloc_func(rb_line_set, rb_line_set_alloc);
    rb_define_method(rb_line_set, "<<", rb_line_set_insert, 1);
    rb_define_method(rb_line_set, "each", rb_line_set_each, 0);
    rb_define_method(rb_line_set, "size", rb_geo_set_size, 0);
    rb_define_method(rb_line_set, "inspect", rb_geo_set_inspect, 0);
    rb_define_method(rb_line_set, "intersects?", rb_line_set_intersects, 1);
    rb_define_method(rb_line_set, "closest_intersection", rb_line_set_closest_intersection, 1);
    rb_define_method(rb_line_set, "slide", rb_line_set_slide, 1);
    rb_define_method(rb_line_set, "include?", rb_line_set_include, 1);
    rb_define_method(rb_line_set, "delete!", rb_line_set_delete, 1);
    rb_define_method(rb_line_set, "intersections", rb_line_set_intersections, 1);
    rb_define_method(rb_line_set, "nr_of_intersections", rb_line_set_n_intersections, 1);
    rb_define_method(rb_line_set, "intersection_free_endpoints", rb_line_set_intersection_free_endpoints, -1);
    rb_define_method(rb_line_set, "clone", rb_line_set_clone, 0);
    rb_define_method(rb_line_set, "_segment_side", rb_geo_set_segment_side, 0);
    rb_define_method(rb_line_set, "_reindex", rb_line_set_reindex, 0);
    rb_define_method(rb_line_set, "_in_segment", rb_line_set_structures_in_segment, 2);
    rb_define_method(rb_line_set, "_segments_for", rb_line_set_segment_ids_for_line, 1);
    rb_define_method(rb_line_set, "_segment_lines", rb_geo_set_segment_lines, 0);
    rb_define_method(rb_line_set, "_bottom_left", rb_geo_set_bottom_left, 0);
    rb_define_method(rb_line_set, "_top_right", rb_geo_set_top_right, 0);
    rb_define_method(rb_line_set, "_width", rb_geo_set_width, 0);
    rb_define_method(rb_line_set, "_height", rb_geo_set_height, 0);
    rb_define_method(rb_line_set, "_with_common_segment", rb_line_set_lines_with_common_segment_id, 1);
    rb_define_method(rb_line_set, "indexed", rb_geo_set_get_indexed, 0);
    rb_define_method(rb_line_set, "indexed=", rb_geo_set_set_indexed, 1);

    rb_define_alloc_func(rb_triangle, rb_triangle_alloc);
    rb_define_method(rb_triangle, "initialize", rb_triangle_initialize, -1);
    rb_define_method(rb_triangle, "inspect", rb_triangle_inspect, 0);
    rb_define_method(rb_triangle, "==", rb_triangle_equals, 1);
    rb_define_method(rb_triangle, "p1", rb_triangle_p1, 0);
    rb_define_method(rb_triangle, "p2", rb_triangle_p2, 0);
    rb_define_method(rb_triangle, "p3", rb_triangle_p3, 0);
    rb_define_method(rb_triangle, "p1=", rb_triangle_set_p1, 0);
    rb_define_method(rb_triangle, "p2=", rb_triangle_set_p2, 0);
    rb_define_method(rb_triangle, "p3=", rb_triangle_set_p3, 0);
    rb_define_method(rb_triangle, "clone", rb_triangle_clone, 0);
    rb_define_method(rb_triangle, "area", rb_triangle_area, 0);
    rb_define_method(rb_triangle, "contains?", rb_triangle_contains, 1);
    rb_define_method(rb_triangle, "intersects?", rb_triangle_intersects, 1);
    rb_define_method(rb_triangle, "overlap", rb_triangle_overlap, 1);
    rb_define_method(rb_triangle, "<=>", rb_triangle_cmp, 1);

    rb_define_alloc_func(rb_line, rb_line_alloc);
    rb_define_method(rb_line, "initialize", rb_line_initialize, -1);
    rb_define_method(rb_line, "inspect", rb_line_inspect, 0);
    rb_define_method(rb_line, "reverse", rb_line_reverse, 0);
    rb_define_method(rb_line, "p1", rb_line_p1, 0);
    rb_define_method(rb_line, "p2", rb_line_p2, 0);
    rb_define_method(rb_line, "p1=", rb_line_set_p1, 1);
    rb_define_method(rb_line, "p2=", rb_line_set_p2, 1);
    rb_define_method(rb_line, "outside?", rb_line_outside, 1);
    rb_define_method(rb_line, "within?", rb_line_within, 1);
    rb_define_method(rb_line, "intersection", rb_line_intersection, 1);
    rb_define_method(rb_line, "==", rb_line_equals, 1);
    rb_define_method(rb_line, "*", rb_line_dot, 1);
    rb_define_method(rb_line, "abs", rb_line_abs, 0);
    rb_define_method(rb_line, "abs=", rb_line_set_abs, 1);
    rb_define_method(rb_line, "angle", rb_line_angle, 0);
    rb_define_method(rb_line, "angle=", rb_line_set_angle, 1);
    rb_define_method(rb_line, "clone", rb_line_clone, 0);
    rb_define_method(rb_line, "side", rb_line_side, 1);
    rb_define_method(rb_line, "contains?", rb_line_contains, 1);
    rb_define_method(rb_line, "parallel", rb_line_parallel, 1);
    rb_define_method(rb_line, "<=>", rb_line_cmp, 1);

    rb_define_alloc_func(rb_point, rb_point_alloc);
    rb_define_method(rb_point, "initialize", rb_point_initialize, 2);
    rb_define_method(rb_point, "inspect", rb_point_inspect, 0);
    rb_define_method(rb_point, "x", rb_point_x, 0);
    rb_define_method(rb_point, "y", rb_point_y, 0);
    rb_define_method(rb_point, "x=", rb_point_set_x, 1);
    rb_define_method(rb_point, "y=", rb_point_set_y, 1);
    rb_define_method(rb_point, "*", rb_point_dot, 1);
    rb_define_method(rb_point, "==", rb_point_equals, 1);
    rb_define_method(rb_point, "abs", rb_point_abs, 0);
    rb_define_method(rb_point, "angle", rb_point_angle, 0);
    rb_define_method(rb_point, "angle=", rb_point_set_angle, 1);
    rb_define_method(rb_point, "abs=", rb_point_set_abs, 1);
    rb_define_method(rb_point, "to", rb_point_to, -1);
    rb_define_method(rb_point, "clone", rb_point_clone, 0);
    rb_define_method(rb_point, "on?", rb_point_on, 1);
    rb_define_method(rb_point, "in?", rb_point_in, 1);
    rb_define_method(rb_point, "<=>", rb_point_cmp, 1);
    rb_define_method(rb_point, "+", rb_point_plus, 1);
    rb_define_method(rb_point, "-", rb_point_minus, 1);
  }
#ifdef __cplusplus
}
#endif

void
init_geo_o() {
}

