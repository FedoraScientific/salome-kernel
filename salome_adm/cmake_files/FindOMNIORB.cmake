#  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
#
#  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
#  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
#
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
#  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#
# -----

FIND_PROGRAM(OMNIORB_IDL omniidl)

# -----

IF(OMNIORB_IDL)
  SET(CORBA_GEN 1)
ELSE(OMNIORB_IDL)
  SET(CORBA_GEN 0)
ENDIF(OMNIORB_IDL)

# -----

SET(OMNIORB_ROOT ${OMNIORB_IDL})
GET_FILENAME_COMPONENT(OMNIORB_ROOT ${OMNIORB_ROOT} PATH)
IF(WINDOWS)
  GET_FILENAME_COMPONENT(OMNIORB_ROOT ${OMNIORB_ROOT} PATH)
ENDIF(WINDOWS)
GET_FILENAME_COMPONENT(OMNIORB_ROOT ${OMNIORB_ROOT} PATH)

# ----

FIND_PATH(OMNIORB_INC1
  NAMES omniORB4/CORBA.h
  PATHS ${OMNIORB_ROOT}/include
  )
FIND_PATH(OMNIORB_INC2
  NAMES CORBA.h
  PATHS ${OMNIORB_ROOT}/include/omniORB4
  )

SET(OMNIORB_INCLUDES -I${OMNIORB_INC1} -I${OMNIORB_INC2})
SET(CORBA_INCLUDES ${OMNIORB_INCLUDES})

# ----

SET(OMNIORB_CXXFLAGS)
SET(OMNIORB_CXXFLAGS ${OMNIORB_CXXFLAGS} -DOMNIORB_VERSION=4)
SET(OMNIORB_CXXFLAGS ${OMNIORB_CXXFLAGS} -D__x86__)

IF(WINDOWS)
  SET(OMNIORB_CXXFLAGS ${OMNIORB_CXXFLAGS} -D__WIN32__)
  #  #  # SET(OMNIORB_CXXFLAGS ${OMNIORB_CXXFLAGS} -D_WIN32_WINNT=0x0400)
  SET(OMNIORB_CXXFLAGS ${OMNIORB_CXXFLAGS} -D__NT__)
  SET(OMNIORB_CXXFLAGS ${OMNIORB_CXXFLAGS} -D__OSVERSION__=4)
ELSE(WINDOWS)
  SET(OMNIORB_CXXFLAGS ${OMNIORB_CXXFLAGS} -D__linux__)
ENDIF(WINDOWS)

SET(CORBA_CXXFLAGS ${OMNIORB_CXXFLAGS})

# ----

IF(WINDOWS)
  FIND_LIBRARY(lib_omniORB4       omniORB4_rtd        ${OMNIORB_ROOT}/lib/x86_win32)
  FIND_LIBRARY(lib_omniDynamic4   omniDynamic4_rtd    ${OMNIORB_ROOT}/lib/x86_win32)
  FIND_LIBRARY(lib_COS4           COS4_rtd            ${OMNIORB_ROOT}/lib/x86_win32)
  FIND_LIBRARY(lib_COSDynamic4    COSDynamic4_rtd     ${OMNIORB_ROOT}/lib/x86_win32)
  FIND_LIBRARY(lib_omnithread     omnithread_rtd      ${OMNIORB_ROOT}/lib/x86_win32)
ELSE(WINDOWS)
  FIND_LIBRARY(lib_omniORB4       omniORB4           ${OMNIORB_ROOT}/lib)
  FIND_LIBRARY(lib_omniDynamic4   omniDynamic4       ${OMNIORB_ROOT}/lib)
  FIND_LIBRARY(lib_COS4           COS4               ${OMNIORB_ROOT}/lib)
  FIND_LIBRARY(lib_COSDynamic4    COSDynamic4        ${OMNIORB_ROOT}/lib)
  FIND_LIBRARY(lib_omnithread     omnithread         ${OMNIORB_ROOT}/lib)
ENDIF(WINDOWS)

SET(OMNIORB_LIBS ${lib_omniORB4} ${lib_omniDynamic4} ${lib_COS4} ${lib_COSDynamic4} ${lib_omnithread})
IF(WINDOWS)
  SET(OMNIORB_LIBS ws2_32.lib mswsock.lib advapi32.lib ${OMNIORB_LIBS})
ENDIF(WINDOWS)

SET(CORBA_LIBS ${OMNIORB_LIBS})

# ----------------

SET(OMNIORB_IDL_PYTHON $ENV{OMNIIDL_PYTHON})
IF(NOT OMNIORB_IDL_PYTHON)
  SET(OMNIORB_IDL_PYTHON ${OMNIORB_IDL})
ENDIF(NOT OMNIORB_IDL_PYTHON)

# ----------------

SET(IDL_CLN_H .hh)
SET(IDL_SRV_H .hh)

# SET(OMNIORB_IDLCXXFLAGS -Wba -nf)
# SET(OMNIORB_IDLPYFLAGS -bpython)
SET(IDLCXXFLAGS -Wba -nf)
SET(IDLPYFLAGS -bpython)
