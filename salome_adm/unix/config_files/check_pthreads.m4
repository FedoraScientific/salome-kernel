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
#@synonpsis CHECK_PTHREADS
dnl  check for pthreads system interfaces.
dnl  set CFLAGS_PTHREADS,  CXXFLAGS_PTHREADS and LIBS_PTHREADS to
dnl  flags to compiler flags for multithread program compilation (if exists),
dnl  and library, if one required.
dnl
dnl@author   (C) Ruslan Shevchenko <Ruslan@Shevchenko.Kiev.UA>, 1998
dnl@id $Id$
dnl ----------------------------------------------------------------
dnl CHECK_PTHREADS
AC_DEFUN(CHECK_PTHREADS,[
AC_CXX_OPTION(-pthread,CPPFLAGS,flag=yes,flag=no)

if test $flag = no; then
  AC_REQUIRE([AC_CANONICAL_SYSTEM])dnl
  AC_CHECK_HEADER(pthread.h,AC_DEFINE(HAVE_PTHREAD_H))
  AC_CHECK_LIB(posix4,nanosleep, LIBS_PTHREADS="-lposix4",LIBS_PTHREADS="")
  AC_CHECK_LIB(pthread,pthread_mutex_lock, 
               LIBS_PTHREADS="-lpthread $LIBS_PTHREADS",LIBS_PTHREADS="")
fi

if test $flag = no && x$LIBS_PTHREADS = x; then
  threads_ok=no
else
  threads_ok=yes
fi
])dnl
dnl
dnl
