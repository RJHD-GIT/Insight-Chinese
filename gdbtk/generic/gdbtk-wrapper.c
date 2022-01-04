/* longjmp-free interface between gdb and gdbtk.
   Copyright (C) 1999-2021 Free Software Foundation, Inc.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

#include "defs.h"
#include "frame.h"
#include "value.h"
#include "varobj.h"
#include "block.h"
#include "exceptions.h"
#include "language.h"
#include "valprint.h"
#include "gdbtk-wrapper.h"


/* Error catcher macro. */
#define GDBTK_CATCH_ERRORS(statement)					\
		try							\
		  {							\
		    statement;						\
		  }							\
                catch (const gdb_exception_error &ex)			\
		  {							\
		    exception_print (gdb_stderr, ex);			\
		    return GDB_ERROR;					\
		  }							\
		return GDB_OK;


gdb_result
GDB_find_relative_frame (struct frame_info *fi, int *start,
			 struct frame_info **result)
{
  GDBTK_CATCH_ERRORS (*result = find_relative_frame (fi, start));
}

gdb_result
GDB_get_current_frame (struct frame_info **result)
{
  GDBTK_CATCH_ERRORS (*result = get_current_frame ());
}

gdb_result
GDB_get_next_frame (struct frame_info *fi, struct frame_info **result)
{
  GDBTK_CATCH_ERRORS (*result = get_next_frame (fi));
}

gdb_result
GDB_get_prev_frame (struct frame_info *fi, struct frame_info **result)
{
  GDBTK_CATCH_ERRORS (*result = get_prev_frame (fi));
}

gdb_result
GDB_varobj_update (struct varobj **varp, int xplicit,
		   std::vector<varobj_update_result> &changes)
{
  GDBTK_CATCH_ERRORS (changes = varobj_update (varp, xplicit));
}
