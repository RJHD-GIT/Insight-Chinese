/* longjmp-free interface between gdb and gdbtk.
   Copyright (C) 1999-2021 Free Software Foundation, Inc.

This file is part of GDB.  It contains routines to safely call common gdb
functions without the fear of longjmp'ing.

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

#ifndef GDBTK_WRAPPER_H
#define GDBTK_WRAPPER_H

/* Whenever any gdb function wrapper is called, its return status is: */
typedef enum gdb_wrapper_status { GDB_OK, GDB_ERROR } gdb_result;

/* This list of functions which have been wrapped. Please keep this list
   in alphabetical order, using "GDB_" to prefix the actual name of the
   function. */
extern gdb_result GDB_find_relative_frame (struct frame_info *fi,
					   int *start,
					   struct frame_info **result);
extern gdb_result GDB_get_current_frame (struct frame_info **result);
extern gdb_result GDB_get_next_frame (struct frame_info *fi,
				      struct frame_info **result);
extern gdb_result GDB_get_prev_frame (struct frame_info *fi,
				      struct frame_info **result);
extern gdb_result GDB_varobj_update (struct varobj **varp, int xplicit,
		             std::vector<varobj_update_result> &changes);

#endif /* GDBTK_WRAPPER_H */
