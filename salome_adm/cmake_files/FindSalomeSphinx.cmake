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

# Sphinx detection for Salome
#
#  !! Please read the generic detection procedure in SalomeMacros.cmake !!
#
# The caller of this macro might set SPHINX_PYTHONPATH to provide a path to be added in front 
# of the PYTHONPATH with which the sphinx command should be ran.
#
# Note the environment is also overriden in 
#     doc/docutils/CMakeLists.txt
# with the creation of a dedicated launcher script.
#

SALOME_FIND_PACKAGE_AND_DETECT_CONFLICTS(Sphinx SPHINX_EXECUTABLE 2)

# Also retrieve paths to DOCUTILS and SETUPTOOLS:
SET(SETUPTOOLS_ROOT_DIR "$ENV{SETUPTOOLS_ROOT_DIR}" CACHE PATH "Path to the Setuptools installation")
SET(DOCUTILS_ROOT_DIR "$ENV{DOCUTILS_ROOT_DIR}" CACHE PATH "Path to the Setuptools installation")

# Ensure the command is run with the given PYTHONPATH
IF(WIN32 AND NOT CYGWIN)
   MESSAGE(WARNING "Sphinx windows command not re-tested.")
   SET(SPHINX_EXECUTABLE ${SPHINX_EXECUTABLE})
ELSE()
   SET(SPHINX_EXECUTABLE /usr/bin/env PYTHONPATH="${SPHINX_PYTHONPATH}:$$PYTHONPATH" ${SPHINX_EXECUTABLE})
ENDIF()

MARK_AS_ADVANCED(SPHINX_EXECUTABLE)
