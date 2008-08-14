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


#ifndef COMMON_H
#define COMMON_H

#include "ruby.h"
#include <stdlib.h>
#include <math.h>
#include <glib.h>
#include <types.h>
#include <point.h>
#include <line.h>
#include <intersection.h>
#include <triangle.h>
#include <geo_set.h>
#include <line_set.h>
#include <point_set.h>
#include <triangle_set.h>

#ifndef M_PI
#define M_PI           3.14159265358979323846  /* pi */
#define M_PI_2         1.57079632679489661923  /* pi/2 */
#define M_PI_4         0.78539816339744830962  /* pi/4 */
#endif

//
// Utility macros.
//

//
// If we have debug turned on when compiling, lets do stuff with all these debug macros.
//
#ifdef GEO_DEBUG
// The indentation of DBEUG messages.
extern gint DEBUG_indentation;
// Only execute s if debugging.
#define IFDEBUG(s) s
// Make the indentation of normal DEBUG messages one greater.
#define INDENT indentation++;
// Make the indentation of normal DEBUG messages one less.
#define UNINDENT indentation--;
// Warn if x is NULL.
#define NULL_WARN(x) if ((x) == NULL) DEBUG("x is NULL!!!");
// Indent DEBUG_indentation spaces and print the debug message.
#define DEBUG(s, ...) {							\
    gint indentation_tmp;						\
    fprintf(stderr, "DEBUG: %s: ", __PRETTY_FUNCTION__);					\
    for (indentation_tmp = 0; indentation_tmp < DEBUG_indentation; indentation_tmp++) fprintf(stderr, " "); \
    fprintf(stderr, s, ## __VA_ARGS__);					\
    fprintf(stderr, "\n");						\
    fflush(NULL); }
// Print out the text in str of length len as a ruby inspected string.
#define STRING_DEBUG(msg,str,len) {					\
    fprintf(stderr, "DEBUG: %s: ", (msg));				\
    rb_funcall(rb_const_get(rb_cObject, rb_intern("Kernel")), rb_intern("print"), 1, rb_funcall(rb_str_new((gchar *) (str), (len)), rb_intern("inspect"), 0)); \
    fprintf(stderr, "\n");						\
    fflush(NULL); }
//
// If we dont have debug turned on while compiling, just replace them with empty semicolons so as not to confuse the compiler.
//
#else
#define INDENT ;
#define UNINDENT ;
#define DEBUG(s, ...) ;
#define BITSTRING_DEBUG(msg,str,len) ;
#define IFDEBUG(s) ;
#endif





#define CLASS(value) (( (struct RBasic *) (value) )->klass)


#define DBL_PREC 0.00001
#define DBL_WITHIN(d1,d2,diff) (((d1) - (d2) < diff) && ((d2) - (d1) < diff))
#define DBL_EQL(d1,d2) (((d1) - (d2) < DBL_PREC) && ((d2) - (d1) < DBL_PREC))
#define DBL_EOG(object,subject) ((subject) - (object) < DBL_PREC)
#define DBL_EOL(object,subject) ((object) - (subject) < DBL_PREC)
#define DBL_GT(object,subject) ((object) - (subject) > DBL_PREC)
#define DBL_LT(object,subject) ((subject) - (object) > DBL_PREC)
#define IS_INTLIKE(x) (DBL_EQL((x), round((x))))
#define CMP(a,b) ((a) - (b))

#define GBOOL2RB(gbool) ((gbool) ? Qtrue : Qfalse)
#define CHECK_NUMERICALITY(v) if (NIL_P((v)) || (TYPE((v)) != T_FIXNUM && TYPE((v)) != T_FLOAT)) rb_raise(rb_eTypeError, "Expected a Number!")



gboolean 
rb_is_a(VALUE obj, VALUE c);

gint
gpointer_compare_as_uint(gconstpointer p1, gconstpointer p2);






#endif


