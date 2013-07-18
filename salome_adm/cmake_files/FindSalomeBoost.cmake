# Copyright (C) 2013  CEA/DEN, EDF R&D, OPEN CASCADE
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
# See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#
# Author: Adrien Bruneton
#

# Boost detection dor Salome
#
#  !! Please read the generic detection procedure in SalomeMacros.cmake !!
#
SET(Boost_USE_STATIC_LIBS        OFF)
SET(Boost_USE_MULTITHREADED      ON )
SET(Boost_USE_STATIC_RUNTIME     OFF)
SET(Boost_NO_BOOST_CMAKE         ON)

SALOME_FIND_PACKAGE_AND_DETECT_CONFLICTS(Boost Boost_INCLUDE_DIRS 1)
IF(Boost_FOUND OR BOOST_FOUND)
   MESSAGE(STATUS "Boost include dirs is: ${Boost_INCLUDE_DIRS}")
ENDIF()   
#MARK_AS_ADVANCED()

## Specific definitions:
IF(WIN32)
  SET(BOOST_DEFINITIONS -DBOOST_DISABLE_ASSERTS)
ENDIF()
