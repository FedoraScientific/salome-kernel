# Copyright (C) 2007-2012  CEA/DEN, EDF R&D, OPEN CASCADE
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

INSTALL(CODE "SET(INSTALL_PYIDL_DIR lib/python${PYTHON_VERSION}/site-packages/salome)") # R1 CHECK
  IF(WINDOWS)
    INSTALL(CODE "SET(INSTALL_PYIDL_DIR bin/salome)") # R1 CHECK
  ENDIF(WINDOWS)

INSTALL(CODE "MACRO(OMNIORB_COMPILE_IDL_FORPYTHON_ON_INSTALL MYOMNIORBIDLPYTHON MYIDLPYFLAGS MYIDLFILE MYFULLDIR)
  FILE(MAKE_DIRECTORY \${MYFULLDIR})
  STRING(REPLACE \" \" \";\" MYIDLPYFLAGS2 \${MYIDLPYFLAGS})
  MESSAGE(STATUS \"Compiling \${MYIDLFILE} into \${MYFULLDIR}\")
  EXECUTE_PROCESS(COMMAND \${MYOMNIORBIDLPYTHON} \${MYIDLPYFLAGS2} -C\${MYFULLDIR} \${MYIDLFILE})
ENDMACRO(OMNIORB_COMPILE_IDL_FORPYTHON_ON_INSTALL)")

# This MACRO uses the following vars
# - OMNIORB_IDL : the idl tool (given by default by FindOMNIORB.cmake)
# - OMNIORB_IDLCXXFLAGS : the options (include excluded) to give to omniidl generator (given by default by FindOMNIORB.cmake)
#
# MYMODULE is a string that will be used to create a target with sources containing *.cc coming from the compilation of ${MYIDLFILES}
# MYIDLFILES containing all idl files to be compiled.
# MYIDLCXXFLAGS containing all directories (only directories) where to find *.idl which depend ${MYIDLFILES}.
MACRO(OMNIORB_ADD_MODULE MYMODULE MYIDLFILES MYIDLCXXFLAGS)
  SET(MYSOURCES)
  FOREACH(input ${MYIDLFILES})
    STRING(REGEX REPLACE ".idl" "" base ${input})
    SET(OMNIORB_IDLCXXFLAGS2 "${OMNIORB_IDLCXXFLAGS}")
    SET(src ${CMAKE_CURRENT_BINARY_DIR}/${base}SK.cc)
    SET(MYSOURCES ${MYSOURCES} ${src})
    SET(outputs ${src})
    SET(dynsrc ${CMAKE_CURRENT_BINARY_DIR}/${base}DynSK.cc)
    SET(outputs ${outputs} ${dynsrc})
    SET(inc ${CMAKE_CURRENT_BINARY_DIR}/${base}.hh)
    SET(outputs ${outputs} ${inc})
    SET(input ${CMAKE_CURRENT_SOURCE_DIR}/${input})
    SET(flags ${OMNIORB_IDLCXXFLAGS2})
    STRING(REGEX MATCH "-bcxx" ISBCXX ${flags})
    IF(NOT ISBCXX)
      SET(flags -bcxx ${flags})
    ENDIF(NOT ISBCXX)
    FOREACH(f ${MYIDLCXXFLAGS})
      SET(flags ${flags} "-I${f}")
    ENDFOREACH(f ${MYIDLCXXFLAGS})
    ADD_CUSTOM_COMMAND(OUTPUT ${outputs}
      COMMAND ${OMNIORB_IDL} ${flags} ${input}
      MAIN_DEPENDENCY ${input})
    SET(IDLPYFLAGS ${flags})
    INSTALL(FILES ${input} DESTINATION idl/salome)
    SET(IDL_HEADER ${CMAKE_CURRENT_BINARY_DIR}/${base}.hh)
    INSTALL(FILES ${IDL_HEADER} DESTINATION include/salome)
    SET(flags)
    FOREACH(f ${MYIDLCXXFLAGS})
      SET(flags "${flags} -I${f}")
    ENDFOREACH(f ${MYIDLCXXFLAGS})
    STRING(REGEX MATCH "-bpython" ISBPYTHON ${flags})
    IF(NOT ISBPYTHON)
      SET(flags "-bpython ${flags}")
    ENDIF(NOT ISBPYTHON)
    SET(IDLPYFLAGS ${flags})
    STRING(REPLACE "\\" "/" IDLPYFLAGS ${IDLPYFLAGS})
    INSTALL(CODE "OMNIORB_COMPILE_IDL_FORPYTHON_ON_INSTALL( \"${OMNIORB_IDL_PYTHON}\" \"${IDLPYFLAGS}\" \"${input}\" \"${CMAKE_INSTALL_PREFIX}/\${INSTALL_PYIDL_DIR}\" )")
  ENDFOREACH(input ${MYIDLFILES})
  ADD_LIBRARY(${MYMODULE} SHARED ${MYSOURCES})
ENDMACRO(OMNIORB_ADD_MODULE)
