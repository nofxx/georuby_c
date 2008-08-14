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

// The indentation of DEBUG messages.
gint DEBUG_indentation = 0;

gboolean
rb_is_a(VALUE obj, VALUE c) {
  VALUE cl = CLASS_OF(obj);
  
  switch (TYPE(c)) {
  case T_MODULE:
  case T_CLASS:
  case T_ICLASS:
    break;
    
  default:
    return FALSE;
  }
  
  while (cl) {
    if (cl == c || RCLASS(cl)->m_tbl == RCLASS(c)->m_tbl)
      return TRUE;
    cl = RCLASS(cl)->super;
  }
  return FALSE;
}

gint
gpointer_compare_as_uint(gconstpointer p1, gconstpointer p2) {
  return GPOINTER_TO_UINT(p1) - GPOINTER_TO_UINT(p2);
}

void
init_common_o() {
}
