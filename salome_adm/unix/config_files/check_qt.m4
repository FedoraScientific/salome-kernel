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

AC_DEFUN([CHECK_QT],[
AC_REQUIRE([AC_PROG_CC])dnl
AC_REQUIRE([AC_PROG_CXX])dnl
AC_REQUIRE([AC_PROG_CPP])dnl
AC_REQUIRE([AC_PROG_CXXCPP])dnl
AC_REQUIRE([CHECK_OPENGL])dnl

AC_CHECKING(for QT)
qt_ok=yes

AC_LANG_SAVE
AC_LANG_CPLUSPLUS

if test "x$QTDIR" = "x"
then
   AC_MSG_RESULT(please define QTDIR variable)
   qt_ok=no
fi

if  test "x$qt_ok" = "xyes"
then
  if test -f ${QTDIR}/bin/moc
  then
    MOC=${QTDIR}/bin/moc
  else
    AC_PATH_PROG(MOC, moc)
  fi
  if test "x$MOC" = "x"
  then
    qt_ok=no
    AC_MSG_RESULT(moc qt-compiler not in PATH variable)
  else
    qt_ok=yes
    AC_MSG_RESULT(moc found)
  fi
fi

if  test "x$qt_ok" = "xyes"
then
  if test -f ${QTDIR}/bin/uic
  then
    UIC=${QTDIR}/bin/uic
  else
    AC_PATH_PROG(UIC, uic)
  fi
  if test "x$UIC" = "x"
  then
    qt_ok=no
    AC_MSG_RESULT(uic qt-interface compiler not in PATH variable)
  else
    qt_ok=yes
    AC_MSG_RESULT(uic found)
  fi
fi

AC_SUBST(QTDIR)
QT_ROOT=$QTDIR

if  test "x$qt_ok" = "xyes"
then
  AC_MSG_CHECKING(include of qt headers)

  CPPFLAGS_old=$CPPFLAGS
  CPPFLAGS="$CPPFLAGS -I$QTDIR/include"

  AC_LANG_CPLUSPLUS
  AC_CHECK_HEADER(qaction.h,qt_ok=yes ,qt_ok=no)

  CPPFLAGS=$CPPFLAGS_old

  if  test "x$qt_ok" = "xno"
  then
    AC_MSG_RESULT(qt headers not found, or too old qt version, in $QTDIR/include)
    AC_MSG_RESULT(QTDIR environment variable may be wrong)
  else
    AC_MSG_RESULT(yes)
       QT_INCLUDES="-I${QT_ROOT}/include -DQT_THREAD_SUPPORT"
    QT_MT_INCLUDES="-I${QT_ROOT}/include -DQT_THREAD_SUPPORT"
  fi
fi

if  test "x$qt_ok" = "xyes"
then
  AC_MSG_CHECKING(linking qt library)
  LIBS_old=$LIBS
  LIBS="$LIBS -L$QTDIR/lib -lqt-mt $OGL_LIBS"

  CXXFLAGS_old=$CXXFLAGS
  CXXFLAGS="$CXXFLAGS -I$QTDIR/include"

  AC_CACHE_VAL(salome_cv_lib_qt,[
    AC_TRY_LINK(
#include <qapplication.h>
,   int n;
    char **s;
    QApplication a(n, s);
    a.exec();,
    eval "salome_cv_lib_qt=yes",eval "salome_cv_lib_qt=no")
  ])
  qt_ok="$salome_cv_lib_qt"

  if  test "x$qt_ok" = "xno"
  then
    AC_MSG_RESULT(unable to link with qt library)
    AC_MSG_RESULT(QTDIR environment variable may be wrong)
  else
    AC_MSG_RESULT(yes)
       QT_LIBS="-L$QTDIR/lib -lqt-mt"
    QT_MT_LIBS="-L$QTDIR/lib -lqt-mt"
  fi

  LIBS=$LIBS_old
  CXXFLAGS=$CXXFLAGS_old

fi

AC_SUBST(MOC)
AC_SUBST(UIC)

AC_SUBST(QT_ROOT)
AC_SUBST(QT_INCLUDES)
AC_SUBST(QT_LIBS)
AC_SUBST(QT_MT_LIBS)

AC_LANG_RESTORE

AC_MSG_RESULT(for qt: $qt_ok)

# Save cache
AC_CACHE_SAVE

])dnl
dnl
