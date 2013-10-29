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

# VTK detection for Salome (see http://www.vtk.org/Wiki/VTK/Build_System_Migration)
#
#  !! Please read the generic detection procedure in SalomeMacros.cmake !!
#

# List the required components:
SET(SalomeVTK_COMPONENTS 
  vtkRenderingFreeTypeOpenGL
  vtkRenderingLOD 
  vtkRenderingAnnotation 
  vtkFiltersParallel
  vtkIOExport
  #vtkWrappingPythonCore  ## ParaView 4.0.1
  vtkWrappingPython
  vtkIOXML
  vtkFiltersVerdict
)

#   Seting the following variable is equivalent to having passed the components
#   when calling the FIND_PACKAGE() command.
SET(SalomeVTK_FIND_COMPONENTS ${SalomeVTK_COMPONENTS})

# If no VTK root dir is specified, try the ParaView root dir:
SET(PARAVIEW_ROOT_DIR "$ENV{PARAVIEW_ROOT_DIR}" CACHE PATH "Path to the ParaView installation")
IF(EXISTS "${PARAVIEW_ROOT_DIR}" AND (NOT VTK_ROOT_DIR))
  MESSAGE(STATUS "Looking for VTK in the ParaView installation (PARAVIEW_ROOT_DIR exists and VTK_ROOT_DIR is not defined) ...")
  
  # Extract sub-directory "paraview-x.xx":
  FILE(GLOB VTK_DIR "${PARAVIEW_ROOT_DIR}/lib/cmake/paraview-*")
  MESSAGE(STATUS "Setting VTK_DIR to: ${VTK_DIR}") 
ENDIF()

SALOME_FIND_PACKAGE_AND_DETECT_CONFLICTS(VTK VTK_INCLUDE_DIRS 2)
MARK_AS_ADVANCED(VTK_DIR)

IF(VTK_FOUND)
  MESSAGE(STATUS "VTK version is ${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}")
  #MESSAGE(STATUS "VTK libraries are: ${VTK_LIBRARIES}")
ENDIF()

IF(VTK_FOUND) 
  SALOME_ACCUMULATE_HEADERS(VTK_INCLUDE_DIRS)
  SALOME_ACCUMULATE_ENVIRONMENT(PATH ${PARAVIEW_ROOT_DIR}/bin)
  SALOME_ACCUMULATE_ENVIRONMENT(LD_LIBRARY_PATH ${PARAVIEW_ROOT_DIR}/lib/paraview-3.98)
  SALOME_ACCUMULATE_ENVIRONMENT(PYTHONPATH ${PARAVIEW_ROOT_DIR}/lib/paraview-3.98
                                           ${PARAVIEW_ROOT_DIR}/lib/paraview-3.98/site-packages
                                           ${PARAVIEW_ROOT_DIR}/lib/paraview-3.98/site-packages/paraview)
  SALOME_ACCUMULATE_ENVIRONMENT(PV_PLUGIN_PATH ${PARAVIEW_ROOT_DIR}/lib/paraview-3.98)
ENDIF()
