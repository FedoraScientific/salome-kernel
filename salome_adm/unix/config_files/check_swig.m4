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

AC_DEFUN([CHECK_SWIG],[
AC_REQUIRE([CHECK_PYTHON])dnl

swig_ok=yes

AC_ARG_WITH(swig,
    [  --with-swig=EXEC swig executable ],
    [SWIG="$withval"
      AC_MSG_RESULT("select $withval as swig executable")
    ], [
      AC_PATH_PROG(SWIG, swig)
    ])

if test "x$SWIG" = "x"
then
    swig_ok=no
    AC_MSG_RESULT(swig not in PATH variable)
fi

if  test "x$swig_ok" = "xyes"
then
   AC_MSG_CHECKING(python wrapper generation with swig)
   cat > conftest.h << EOF
int f(double);
EOF

   $SWIG -module conftest -python conftest.h >/dev/null 2>&1
   if test -f conftest_wrap.c
   then
      SWIG_FLAGS="-c++ -python -shadow"
   else
      swig_ok=no  
   fi
   rm -f conftest*
   AC_MSG_RESULT($swig_ok) 
fi

AC_SUBST(SWIG_FLAGS)
AC_SUBST(SWIG)

AC_MSG_RESULT(for swig: $swig_ok)

])dnl
dnl
