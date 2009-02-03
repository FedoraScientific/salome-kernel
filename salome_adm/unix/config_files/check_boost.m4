dnl  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
dnl
dnl  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
dnl  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
dnl
AC_DEFUN([CHECK_BOOST],[

AC_CHECKING(for BOOST Library)

AC_REQUIRE([ENABLE_PTHREADS])dnl

AC_LANG_SAVE
AC_LANG_CPLUSPLUS

BOOST_CPPFLAGS=""
BOOST_LIBSUFFIX="-mt"
BOOST_LIBS=""

AC_CHECKING(for BOOST location)
AC_ARG_WITH(boost,
   [AC_HELP_STRING([--with-boost=DIR],[root directory path to BOOST library installation])],
   [BOOSTDIR="$withval"
    AC_MSG_RESULT("select $withval as path to BOOST library")
   ])
   
if test "x${BOOSTDIR}" = "x" ; then
  BOOSTDIR="/usr"
fi

AC_MSG_RESULT(\$BOOSTDIR = ${BOOSTDIR})

CPPFLAGS_old="${CPPFLAGS}"
LIBS_old=$LIBS

if test "x${BOOSTDIR}" != "x" ; then
  BOOST_CPPFLAGS="-I${BOOSTDIR}/include"
  BOOST_LIBS="-L${BOOSTDIR}/lib${LIB_LOCATION_SUFFIX}"
fi

if test "x${BOOSTDIR}" = "x/usr" ; then
  BOOST_CPPFLAGS=""
  BOOST_LIBS=""
fi

boost_ok=no
boost_headers_ok=no
boost_binaries_ok=no

dnl BOOST headers
AC_CHECKING(for BOOST headers)
CPPFLAGS="${CPPFLAGS_old} ${BOOST_CPPFLAGS}"

boost_include_dir_ok=yes
if test "x${BOOSTDIR}" != "x" ; then
  AC_CHECK_FILE(${BOOSTDIR}/include/boost/shared_ptr.hpp,
                boost_include_dir_ok=yes,
                boost_include_dir_ok=no)
fi

BOOST_PROGRAM_OPTIONS_LIB=no
if test "x${boost_include_dir_ok}" = "xyes" ; then
  AC_CHECK_FILE(${BOOSTDIR}/include/boost/program_options.hpp,
                BOOST_PROGRAM_OPTIONS_LIB=yes,
                BOOST_PROGRAM_OPTIONS_LIB=no)
fi
AC_MSG_RESULT(for boost program_options tool: $BOOST_PROGRAM_OPTIONS_LIB)

if test "x${boost_include_dir_ok}" = "xyes" ; then
  AC_TRY_COMPILE([#include <boost/shared_ptr.hpp>],
                 [boost::shared_ptr<int>(new int)],
                 boost_headers_ok=yes,
                 boost_headers_ok=no)
fi

if test "x${boost_headers_ok}" = "xno" ; then
  BOOST_CPPFLAGS="BOOST_CPPFLAGS_NOT_DEFINED"
else
  AC_MSG_RESULT(\$BOOST_CPPFLAGS = ${BOOST_CPPFLAGS})
fi
AC_MSG_RESULT(for boost headers: $boost_headers_ok)

if test "x${boost_headers_ok}" = "xyes" ; then
  dnl BOOST binaries
  AC_CHECKING(for BOOST binaries)
  boost_lib_dir_ok=yes
  if test "x${BOOSTDIR}" != "x" ; then
    AC_CHECK_FILE(${BOOSTDIR}/lib${LIB_LOCATION_SUFFIX}/libboost_thread${BOOST_LIBSUFFIX}.so,
                  boost_lib_dir_ok=yes,
                  boost_lib_dir_ok=no)
    if test "x${boost_lib_dir_ok}" = "xno" ; then
      BOOST_LIBSUFFIX=""
      AC_CHECK_FILE(${BOOSTDIR}/lib${LIB_LOCATION_SUFFIX}/libboost_thread${BOOST_LIBSUFFIX}.so,
                    boost_lib_dir_ok=yes,
                    boost_lib_dir_ok=no)
    fi
  fi
  if test "x${boost_lib_dir_ok}" = "xyes" ; then
    LIBS="${LIBS_old} ${BOOST_LIBS} -lboost_thread${BOOST_LIBSUFFIX}"
    AC_TRY_LINK([#include <boost/thread/thread.hpp>],
                [struct TBody{ void operator()(){} }; boost::thread(TBody())],
                boost_binaries_ok=yes,
                boost_binaries_ok=no)
    if test "x${boost_binaries_ok}" = "xno" ; then
      BOOST_LIBSUFFIX=""
      LIBS="${LIBS_old} ${BOOST_LIBS} -lboost_thread${BOOST_LIBSUFFIX}"
      AC_TRY_LINK([#include <boost/thread/thread.hpp>],
                  [struct TBody{ void operator()(){} }; boost::thread(TBody())],
                  boost_binaries_ok=yes,
                  boost_binaries_ok=no)
    fi
  fi
fi

if test "x${boost_binaries_ok}" = "xno" ; then
  BOOST_LIBS="BOOST_LIBS_NOT_FOUND"
  BOOST_LIBSUFFIX="-not-defined"
else
  AC_MSG_RESULT(\$BOOST_LIBSUFFIX = ${BOOST_LIBSUFFIX})
  AC_MSG_RESULT(\$BOOST_LIBS = ${BOOST_LIBS})
fi
AC_MSG_RESULT(for boost binaries: $boost_binaries_ok)

CPPFLAGS="${CPPFLAGS_old}"
LIBS="${LIBS_old}"

if test "x${boost_headers_ok}" = "xyes" ; then
  if test "x${boost_binaries_ok}" = "xyes" ; then
    boost_ok=yes
  fi
fi

AC_MSG_RESULT(for boost: $boost_ok)

AC_SUBST(BOOST_CPPFLAGS)
AC_SUBST(BOOST_LIBSUFFIX)
AC_SUBST(BOOST_LIBS)
AC_SUBST(BOOST_PROGRAM_OPTIONS_LIB)

AC_LANG_RESTORE

])dnl


