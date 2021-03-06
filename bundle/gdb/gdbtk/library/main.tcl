# GDBtk (Insight) entry point
# Copyright (C) 1997-2015 Red Hat, Inc.
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License (GPL) as published by
# the Free Software Foundation; either version 2 of the License, or (at
# your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.


# State is controlled by 5 global boolean variables.
#
# gdb_target_changed
# gdb_exe_changed
# gdb_running
# gdb_downloading
# gdb_loaded

################### Initialization code #########################

# If GDBtk fails to start at all, you might want to uncomment one or
# both of these.
#set tcl_traceExec 2
#set tcl_traceCompile 1

# Add gdb's Tcl library directory to the end of the auto-load search path, if
# it isn't already on the path.
# Also, add the plugins directory if it exists.
# Note: GDBTK_LIBRARY will be set in tcl_findLibrary before main.tcl is called.

set gdb_plugins ""
if {[info exists auto_path]} {
  if {[lsearch -exact $auto_path $GDBTK_LIBRARY] < 0} {
    lappend auto_path $GDBTK_LIBRARY
  }

  # Add default plugins directory, which will be [name of exe]/../../lib/insight1.0
  set exename [info nameofexecutable]
  set dir [file join [file dirname [file dirname $exename]] lib insight1.0]
  if {[file exists $dir]} {
    lappend gdb_plugins $dir
    lappend auto_path $dir
  }
  # Add any user-specified plugins directories
  if {[info exists env(INSIGHT_PLUGINS)]} {
    set dirs [split $env(INSIGHT_PLUGINS) :]
    lappend gdb_plugins $dirs
    lappend auto_path $dirs
  }
}

# Require the packages we need.  Most are loaded already, but this will catch
# any odd errors... :

foreach p {{tcl 8.5} {tk 8.5} {itcl 3.3} {itk 3.3} {iwidgets 4.0} {gdbtk 1.0} {combobox 2.2} {debugger 1.0}} {
  set failed [catch {package require [lindex $p 0]} msg]
  if {$failed} {
    set failed [catch {package require [string toupper [lindex $p 0] 0 0]} msg]
  }
  if {!$failed} {
    set failed [expr [package vcompare $msg [lindex $p 1]] < 0]
  }
  if {$failed} {
    catch {package require [lindex $p 0] [lindex $p 1]} msg
    if {![info exists ::env(GDBTK_TEST_RUNNING)] || $::env(GDBTK_TEST_RUNNING) == 0} {
      if {$::tcl_platform(platform) != "windows"} {
        puts stderr "Error: $msg"
      }
      catch {tk_messageBox -title Error -message $msg -icon error -type ok}
    }
    exit -1
  }
  #puts "Loaded [lindex $p 0] $msg"
}

namespace import itcl::*
namespace import debugger::*

# Environment variables controlling debugging:
# GDBTK_TRACE
#	unset or 0	no tracing
#	1		tracing initialized but not started
#	2		tracing initialized and started
#
# GDBTK_DEBUGFILE - filename to write debugging messages and
#	trace information (if tracing is enabled).
#
if {[info exists env(GDBTK_TRACE)] && $env(GDBTK_TRACE) != 0} {
  # WARNING: the tracing code must not trace into itself or
  # infinite recursion will result. As currently configured
  # the tracing code will not trace basic tcl functions or anything defined
  # before debugger::init.  For this reason we must source the DebugWin
  # code before debugger::init is called.
  source [file join $GDBTK_LIBRARY debugwin.ith]
  source [file join $GDBTK_LIBRARY debugwin.itb]

  # Calling this installs our hooks for tracing and profiling.
  # This WILL slow things down.
  ::debugger::init

  if {$env(GDBTK_TRACE) == 2} {
    ::debugger::trace_start
  }
}

if {[info exists env(GDBTK_DEBUG)] && $env(GDBTK_DEBUG) != 0} {
  if {[info exists env(GDBTK_DEBUGFILE)]} {
    ::debugger::logfile $env(GDBTK_DEBUGFILE)
  } else {
    ::debugger::logfile "insight.log"
  }
}

# For testing
set _test(interactive) 0

# Set up platform globals. We replace Tcl's tcl_platform with
# our own version which knows the difference between cygwin and
# mingw.
global gdbtk_platform
set gdbtk_platform(platform) $tcl_platform(platform)
switch $tcl_platform(platform) {
  windows {
    if {[llength [info commands ide_cygwin_path]] == 0} {
      set gdbtk_platform(os) "mingw"
    } else {
      set gdbtk_platform(os) "cygwin"
    }
  }

  default {
    set gdbtk_platform(os) $tcl_platform(os)
  }
}
set gdbtk_platform(osVersion) $tcl_platform(osVersion)

# set traces on state variables
trace variable gdb_running w do_state_hook
trace variable gdb_downloading w do_state_hook
trace variable gdb_loaded w do_state_hook
define_hook state_hook

# set up preferences
pref init

# let libgui tell us how to feel
standard_look_and_feel

# now let GDB set its default preferences
pref_set_defaults

# read in preferences
pref_read

init_disassembly_flavor

# initialize state variables
initialize_gdbtk

# Arrange for session code to notice when file changes.
add_hook file_changed_hook Session::notice_file_change

ManagedWin::init

# This stuff will help us play nice with WindowMaker's AppIcons.
# Can't do the first bit yet, since we don't get this from gdb...
# wm command . [concat $argv0 $argv]
wm group . .

# some initial commands to get gdb in the right mode
gdb_cmd {set height 0}
gdb_cmd {set width 0}

if {[info exists env(GDBTK_TEST_RUNNING)] && $env(GDBTK_TEST_RUNNING)} {
  set gdb_target_name "exec"
} else {
  # gdb_target_name is the name of the GDB target; that is, the argument
  # to the GDB target command.
  set gdb_target_name ""
  # By setting gdb_target_changed, we force a target dialog
  # to be displayed on the first "run"
  set gdb_target_changed 1
}

update

# Uncomment the next line if you want a splash screen at startup...
# ManagedWin::open About -transient -expire 5000

# initialize IPC to enable multiple Insight's to communicate
# with each other.
set iipc 0
if {[pref get gdb/ipc/enabled]} {
  set ::insight_ipc [Iipc \#auto]
}

gdbtk_idle

