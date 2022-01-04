/* tclwinpath.c -- Tcl routines to convert paths under cygwin.
   Copyright (C) 1997 Cygnus Solutions.
   Written by Ian Lance Taylor <ian@cygnus.com>.

   This file contains Tcl interface routines to do path translation
   when using cygwin.  */

#ifdef __CYGWIN__

#include <windows.h>
#include <sys/cygwin.h>

#include <tcl.h>

#include "guitcl.h"
#include "subcommand.h"


/* This file declares a Tcl command with subcommands.

   Each of the following subcommands returns a string based on the
   PATH argument.  If PATH is already in the desired form, these
   commands just return it unchanged.

   ide_cygwin_path to_win32 PATH
       Return PATH converted to a win32 pathname.

   ide_cygwin_path to_full_win32 PATH
       Return PATH converted to an absolute win32 pathname.

   ide_cygwin_path to_posix PATH
       Return PATH converted to a POSIX pathname.

   ide_cygwin_path to_full_posix PATH
       Return PATH converted to an absolute POSIX pathname.

   The following subcommand returns a boolean value.

   ide_cygwin_path posix_path_list_p PATHLIST
       Return whether PATHLIST is a POSIX style path list.

   The following subcommands return strings.

   ide_cygwin_path posix_to_win32_path_list PATHLIST
       Return PATHLIST converted from POSIX style to win32 style.

   ide_cygwin_path win32_to_posix_path_list PATHLIST
       Return PATHLIST converted from win32 style to POSIX style.

   */

static void conv_path(Tcl_Interp *interp,
		      cygwin_conv_path_t what, const char *arg)
{
  ssize_t size = cygwin_conv_path (what, arg, NULL, 0);

  Tcl_ResetResult (interp);
  if (size >= 0)
    {
      char *to = ckalloc (size);
      cygwin_conv_path (what, arg, to, size);
      Tcl_SetResult (interp, to, TCL_DYNAMIC);
    }
}

static void conv_path_list(Tcl_Interp *interp,
			   cygwin_conv_path_t what, const char *arg)
{
  ssize_t size = cygwin_conv_path_list (what, arg, NULL, 0);

  Tcl_ResetResult (interp);
  if (size >= 0)
    {
      char *to = ckalloc (size);
      cygwin_conv_path_list (what, arg, to, size);
      Tcl_SetResult (interp, to, TCL_DYNAMIC);
    }
}
  
  
/* Handle ide_cygwin_path to_win32.  */

static int
path_to_win32 (ClientData cd, Tcl_Interp *interp, int argc, const char **argv)
{
  conv_path(interp, CCP_POSIX_TO_WIN_A | CCP_RELATIVE, argv[2]);
  return TCL_OK;
}

/* Handle ide_cygwin_path to_full_win32.  */

static int
path_to_full_win32 (ClientData cd, Tcl_Interp *interp,
		    int argc, const char **argv)
{
  conv_path(interp, CCP_POSIX_TO_WIN_A | CCP_ABSOLUTE, argv[2]);
  return TCL_OK;
}

/* Handle ide_cygwin_path to_posix.  */

static int
path_to_posix (ClientData cd, Tcl_Interp *interp, int argc, const char **argv)
{
  conv_path(interp, CCP_WIN_A_TO_POSIX | CCP_RELATIVE, argv[2]);
  return TCL_OK;
}

/* Handle ide_cygwin_path to_full_posix.  */

static int
path_to_full_posix (ClientData cd, Tcl_Interp *interp,
		    int argc, const char **argv)
{
  conv_path(interp, CCP_WIN_A_TO_POSIX | CCP_ABSOLUTE, argv[2]);
  return TCL_OK;
}

/* Handle ide_cygwin_path posix_path_list_p.  */

static int
path_posix_path_list_p (ClientData cd, Tcl_Interp *interp,
			int argc, const char **argv)
{
  int ret;

  ret = cygwin_posix_path_list_p (argv[2]);
  Tcl_ResetResult (interp);
  Tcl_SetBooleanObj (Tcl_GetObjResult (interp), ret);
  return TCL_OK;
}

/* Handle ide_cygwin_path posix_to_win32_path_list.  */

static int
path_posix_to_win32_path_list (ClientData cd, Tcl_Interp *interp,
			       int argc, const char **argv)
{
  conv_path_list (interp, CCP_POSIX_TO_WIN_A | CCP_RELATIVE, argv[2]);
  return TCL_OK;
}

/* Handle ide_cygwin_path win32_to_posix_path_list.  */

static int
path_win32_to_posix_path_list (ClientData cd, Tcl_Interp *interp,
			       int argc, const char **argv)
{
  conv_path_list (interp, CCP_WIN_A_TO_POSIX | CCP_RELATIVE, argv[2]);
  return TCL_OK;
}

/* The subcommand table.  */

static const struct ide_subcommand_table path_commands[] =
{
  { "to_win32",		path_to_win32,		3, 3 },
  { "to_full_win32",	path_to_full_win32,	3, 3 },
  { "to_posix",		path_to_posix,		3, 3 },
  { "to_full_posix",	path_to_full_posix,	3, 3 },
  { "posix_path_list_p", path_posix_path_list_p, 3, 3 },
  { "posix_to_win32_path_list", path_posix_to_win32_path_list, 3, 3 },
  { "win32_to_posix_path_list", path_win32_to_posix_path_list, 3, 3 },
  { NULL, NULL, 0, 0}
};

/* Create the ide_cygwin_path command.  */

int
ide_create_cygwin_path_command (Tcl_Interp *interp)
{
  return ide_create_command_with_subcommands (interp, "ide_cygwin_path",
					      path_commands, NULL, NULL);
}

#endif /* __CYGWIN__ */
