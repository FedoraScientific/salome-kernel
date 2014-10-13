# Copyright (C) 2013-2014  CEA/DEN, EDF R&D, OPEN CASCADE
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
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

IF(EXISTS "/usr/share/cmake/paraview")
  MESSAGE(STATUS "Found Paraview bundled VTK") 
  SET(VTK_USE_FILE /usr/share/cmake/paraview/UseVTK.cmake)
  SET(VTK_INCLUDE_DIRS /usr/include/paraview)
  SET(VTK_LIBRARY_DIR /usr/lib${LIB_SUFFIX}/paraview)
  
  FIND_LIBRARY(VTK_RENDERING_FREETYPEOPENGL vtkRenderingFreeTypeOpenGL ${VTK_LIBRARY_DIR})
  FIND_LIBRARY(VTK_RENDERING_LOD vtkRenderingLOD ${VTK_LIBRARY_DIR})
  FIND_LIBRARY(VTK_RENDERING_ANNOTATION vtkRenderingAnnotation ${VTK_LIBRARY_DIR})
  FIND_LIBRARY(VTK_FILTERS_PARALLEL vtkFiltersParallel ${VTK_LIBRARY_DIR})
  FIND_LIBRARY(VTK_IO_EXPORT vtkIOExport ${VTK_LIBRARY_DIR})
  FIND_LIBRARY(VTK_WRAPPING_PYTHONCORE vtkWrappingPython27Core ${VTK_LIBRARY_DIR})
  FIND_LIBRARY(VTK_IO_XML vtkIOXML ${VTK_LIBRARY_DIR})
  FIND_LIBRARY(VTK_FILTERS_VERDICT vtkFiltersVerdict ${VTK_LIBRARY_DIR})
  FIND_LIBRARY(VTK_RENDERING_LABEL vtkRenderingLabel ${VTK_LIBRARY_DIR})
  FIND_LIBRARY(VTK_INTERACTION_WIDGETS vtkInteractionWidgets ${VTK_LIBRARY_DIR})
  FIND_LIBRARY(VTK_INFOVIS_CORE vtkInfovisCore ${VTK_LIBRARY_DIR})
  
  SET(VTK_LIBRARIES
    ${VTK_RENDERING_FREETYPEOPENGL}
    ${VTK_RENDERING_LOD}
    ${VTK_RENDERING_ANNOTATION}
    ${VTK_FILTERS_PARALLEL}
    ${VTK_IO_EXPORT}
    ${VTK_WRAPPING_PYTHONCORE}
    ${VTK_IO_XML}
    ${VTK_FILTERS_VERDICT}
    ${VTK_RENDERING_LABEL}
    ${VTK_INTERACTION_WIDGETS}
    ${VTK_INFOVIS_CORE})

  SALOME_ACCUMULATE_HEADERS(${VTK_INCLUDE_DIRS})
  SALOME_ACCUMULATE_ENVIRONMENT(LD_LIBRARY_PATH ${VTK_LIBRARY_DIR})
  SALOME_ACCUMULATE_ENVIRONMENT(PYTHONPATH ${VTK_LIBRARY_DIR})
  SALOME_ACCUMULATE_ENVIRONMENT(PV_PLUGIN_PATH ${VTK_LIBRARY_DIR})
  SET(VTK_FOUND TRUE)
ENDIF()
