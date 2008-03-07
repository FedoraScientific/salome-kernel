dnl  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
dnl  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
dnl 
dnl  This library is free software; you can redistribute it and/or 
dnl  modify it under the terms of the GNU Lesser General Public 
dnl  License as published by the Free Software Foundation; either 
dnl  version 2.1 of the License. 
dnl 
dnl  This library is distributed in the hope that it will be useful, 
dnl  but WITHOUT ANY WARRANTY; without even the implied warranty of 
dnl  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
dnl  Lesser General Public License for more details. 
dnl 
dnl  You should have received a copy of the GNU Lesser General Public 
dnl  License along with this library; if not, write to the Free Software 
dnl  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
dnl 
dnl  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
dnl
dnl
dnl
dnl define macros :
dnl AC_ENABLE_PRODUCTION AC_DISABLE_PRODUCTION
dnl and 
dnl AC_ENABLE_DEBUG AC_DISABLE_DEBUG
dnl
dnl version $Id$
dnl author Patrick GOLDBRONN
dnl
 
dnl AC_ENABLE_PRODUCTION
dnl
dnl This macro enables production build : optimized + no debugging information (-g)
dnl default = not enabled
dnl
AC_DEFUN([AC_ENABLE_PRODUCTION],
[define([AC_ENABLE_PRODUCTION_DEFAULT], ifelse($1, no, no, yes))dnl
AC_ARG_ENABLE([production],
    [AC_HELP_STRING([--enable-production],
     [compile in optimized mode @<:@default=]AC_ENABLE_PRODUCTION_DEFAULT[@:>@])],
    [
    enable_production=$enableval
    ],
    [enable_production=]AC_ENABLE_PRODUCTION_DEFAULT)

AC_CXX_OPTION(-Wno-deprecated,CXXFLAGS)
AC_CXX_OPTION(-Wparentheses,CXXFLAGS)
AC_CXX_OPTION(-Wreturn-type,CXXFLAGS)
AC_CXX_OPTION(-Wmissing-declarations,CXXFLAGS)
AC_CXX_OPTION(-fmessage-length=0,CXXFLAGS)
AC_CXX_OPTION(-Wunused,CXXFLAGS)
AC_CXX_OPTION(-pipe,CXXFLAGS)

if test "X$enable_production" = "Xyes"; then
  CFLAGS="$CFLAGS -O"
  AC_CXX_OPTION(-Wuninitialized,CXXFLAGS)
  CXXFLAGS="$CXXFLAGS -O "
else
  CFLAGS="$CFLAGS -g"
  CXXFLAGS="$CXXFLAGS -g"
fi
])

# AC_DISABLE_PRODUCTION - set the default flag to --disable-production
AC_DEFUN([AC_DISABLE_PRODUCTION], [AC_ENABLE_PRODUCTION(no)])

dnl AC_ENABLE_DEBUG
dnl
dnl This macro enables debugging build : debug + trace
dnl default = not enabled
dnl
AC_DEFUN([AC_ENABLE_DEBUG],
    [define([AC_ENABLE_DEBUG_DEFAULT], ifelse($1, no, no, yes))dnl
    AC_ARG_ENABLE([debug],
       [AC_HELP_STRING([--enable-debug],
             [build with debug and trace information @<:@default=]AC_ENABLE_DEBUG_DEFAULT[@:>@])],
    [
    enable_debug=$enableval
    ],
    [enable_debug=]AC_ENABLE_DEBUG_DEFAULT)

if test "X$enable_debug" = "Xyes"; then
  CFLAGS="$CFLAGS -g -D_DEBUG_ "
  CXXFLAGS="$CXXFLAGS -g -D_DEBUG_ "
fi
])

# AC_DISABLE_DEBUG - set the default flag to --disable-debug
AC_DEFUN([AC_DISABLE_DEBUG], [AC_ENABLE_DEBUG(no)])
