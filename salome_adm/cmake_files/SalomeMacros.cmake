# Copyright (C) 2012-2013  CEA/DEN, EDF R&D, OPEN CASCADE
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
# Author: A.Geay, V. Sandler, A. Bruneton
#

#----------------------------------------------------------------------------
# LIST_CONTAINS is a macro useful for determining whether a list has a 
# particular entry
#----------------------------------------------------------------------------
MACRO(LIST_CONTAINS var value)
  SET(${var})
  FOREACH(value2 ${ARGN})
    IF(${value} STREQUAL "${value2}")
      SET(${var} TRUE)
    ENDIF (${value} STREQUAL "${value2}")
  ENDFOREACH (value2)
ENDMACRO(LIST_CONTAINS)

#----------------------------------------------------------------------------
# The PARSE_ARGUMENTS macro will take the arguments of another macro and
# define several variables.
#
# USAGE:  PARSE_ARGUMENTS(prefix arg_names options arg1 arg2...)
#
# ARGUMENTS:
#
# prefix: IN: a prefix to put on all variables it creates.
#
# arg_names: IN: a list of names.
# For each item in arg_names, PARSE_ARGUMENTS will create a 
# variable with that name, prefixed with prefix_. Each variable will be filled
# with the arguments that occur after the given arg_name is encountered
# up to the next arg_name or the end of the arguments. All options are
# removed from these lists. PARSE_ARGUMENTS also creates a
# prefix_DEFAULT_ARGS variable containing the list of all arguments up
# to the first arg_name encountered.
#
# options: IN: a list of options.
# For each item in options, PARSE_ARGUMENTS will create a
# variable with that name, prefixed with prefix_. So, for example, if prefix is
# MY_MACRO and options is OPTION1;OPTION2, then PARSE_ARGUMENTS will
# create the variables MY_MACRO_OPTION1 and MY_MACRO_OPTION2. These
# variables will be set to true if the option exists in the command line
# or false otherwise.
# arg_names and options lists should be quoted.
#
# The rest of PARSE_ARGUMENTS are arguments from another macro to be parsed.
#----------------------------------------------------------------------------
MACRO(PARSE_ARGUMENTS prefix arg_names option_names)
  SET(DEFAULT_ARGS)
  FOREACH(arg_name ${arg_names})
    SET(${prefix}_${arg_name})
  ENDFOREACH(arg_name)
  FOREACH(option ${option_names})
    SET(${prefix}_${option} FALSE)
  ENDFOREACH(option)
  SET(current_arg_name DEFAULT_ARGS)
  SET(current_arg_list)
  FOREACH(arg ${ARGN})
    LIST_CONTAINS(is_arg_name ${arg} ${arg_names})
    IF (is_arg_name)
      SET(${prefix}_${current_arg_name} ${current_arg_list})
      SET(current_arg_name ${arg})
      SET(current_arg_list)
    ELSE (is_arg_name)
      LIST_CONTAINS(is_option ${arg} ${option_names})
      IF (is_option)
      SET(${prefix}_${arg} TRUE)
      ELSE (is_option)
      SET(current_arg_list ${current_arg_list} ${arg})
      ENDIF (is_option)
    ENDIF (is_arg_name)
  ENDFOREACH(arg)
  SET(${prefix}_${current_arg_name} ${current_arg_list})
ENDMACRO(PARSE_ARGUMENTS)

#----------------------------------------------------------------------------
# SALOME_INSTALL_SCRIPTS is a macro useful for installing scripts.
#
# USAGE: SALOME_INSTALL_SCRIPTS(file_list path [WORKING_DIRECTORY dir] [DEF_PERMS])
#
# ARGUMENTS:
# file_list: IN : list of files to be installed. This list should be quoted.
# path: IN : full pathname for installing.
# 
# By default files to be installed as executable scripts.
# If DEF_PERMS option is provided, than permissions for installed files are
# only OWNER_WRITE, OWNER_READ, GROUP_READ, and WORLD_READ. 
#----------------------------------------------------------------------------
MACRO(SALOME_INSTALL_SCRIPTS file_list path)
  PARSE_ARGUMENTS(SALOME_INSTALL_SCRIPTS "WORKING_DIRECTORY" "DEF_PERMS" ${ARGN})
  SET(PERMS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)
  IF(NOT SALOME_INSTALL_SCRIPTS_DEF_PERMS)
    SET(PERMS ${PERMS} OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE)
  ENDIF(NOT SALOME_INSTALL_SCRIPTS_DEF_PERMS)
  FOREACH(file ${file_list})
    SET(PREFIX "")
    IF(IS_ABSOLUTE ${file})
      GET_FILENAME_COMPONENT(file_name ${file} NAME)
    ELSE()
      SET(file_name ${file})
      IF(SALOME_INSTALL_SCRIPTS_WORKING_DIRECTORY)
	SET(PREFIX "${SALOME_INSTALL_SCRIPTS_WORKING_DIRECTORY}/")
      ENDIF(SALOME_INSTALL_SCRIPTS_WORKING_DIRECTORY)
    ENDIF(IS_ABSOLUTE ${file})
    INSTALL(FILES ${PREFIX}${file} DESTINATION ${path} PERMISSIONS ${PERMS})
    GET_FILENAME_COMPONENT(ext ${file} EXT)
    IF(ext STREQUAL .py)
      INSTALL(CODE "MESSAGE(STATUS \"py compiling ${CMAKE_INSTALL_PREFIX}/${path}/${file_name}\")")
      INSTALL(CODE "SET(CMD \"import py_compile ; py_compile.compile('${CMAKE_INSTALL_PREFIX}/${path}/${file_name}')\")")
      INSTALL(CODE "EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} -c \"\${CMD}\")")
      INSTALL(CODE "EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} -O -c \"\${CMD}\")")
    ENDIF(ext STREQUAL .py)
  ENDFOREACH(file ${file_list})
ENDMACRO(SALOME_INSTALL_SCRIPTS)

#----------------------------------------------------------------------------
# SALOME_INSTALL_SCRIPTS is a macro useful for installing executable scripts.
# ARGUMENTS:
# PYFILE2COMPINST: IN : list of python files to be installed.
# PYFILELOC: IN : full pathname for installing.
# Permissions of installed files: OWNER_WRITE, OWNER_READ, GROUP_READ, and WORLD_READ
#----------------------------------------------------------------------------
MACRO(INSTALL_AND_COMPILE_PYTHON_FILE PYFILE2COMPINST PYFILELOC)
  INSTALL(CODE "SET(PYTHON_FILE ${f})")
  FOREACH(input ${PYFILE2COMPINST})
    GET_FILENAME_COMPONENT(inputname ${input} NAME)
    INSTALL(FILES ${input} DESTINATION ${CMAKE_INSTALL_PREFIX}/${PYFILELOC})
    INSTALL(CODE "MESSAGE(STATUS \"py compiling ${CMAKE_INSTALL_PREFIX}/${PYFILELOC}/${inputname}\")")
    INSTALL(CODE "SET(CMD \"import py_compile ; py_compile.compile('${CMAKE_INSTALL_PREFIX}/${PYFILELOC}/${inputname}')\")")
    INSTALL(CODE "EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} -c \"\${CMD}\")")
    INSTALL(CODE "EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} -O -c \"\${CMD}\")")
  ENDFOREACH(input ${PYFILE2COMPINST})
ENDMACRO(INSTALL_AND_COMPILE_PYTHON_FILE PYFILE2COMPINST PYFILELOC)

#----------------------------------------------------------------------------
# SALOME_CONFIGURE_FILE is a macro useful for copying a file to another location 
# and modify its contents.
#
# USAGE: SALOME_CONFIGURE_FILE(in_file out_file [INSTALL dir])
#
# ARGUMENTS:
# in_file: IN : input file (if relative path is given, full file path is computed from current source dir).
# out_file: IN : output file (if relative path is given, full file path is computed from current build dir).
# If INSTALL is specified, then 'out_file' will be installed to the 'dir' directory.
#----------------------------------------------------------------------------
MACRO(SALOME_CONFIGURE_FILE IN_FILE OUT_FILE)
  IF(IS_ABSOLUTE ${IN_FILE})
    SET(_in_file ${IN_FILE})
  ELSE()
    SET(_in_file ${CMAKE_CURRENT_SOURCE_DIR}/${IN_FILE})
  ENDIF()
  IF(IS_ABSOLUTE  ${OUT_FILE})
    SET(_out_file ${OUT_FILE})
  ELSE()
    SET(_out_file ${CMAKE_CURRENT_BINARY_DIR}/${OUT_FILE})
  ENDIF()
  MESSAGE(STATUS "Creation of ${_out_file}")
  CONFIGURE_FILE(${_in_file} ${_out_file} @ONLY)
  PARSE_ARGUMENTS(SALOME_CONFIGURE_FILE "INSTALL" "" ${ARGN})
  IF(SALOME_CONFIGURE_FILE_INSTALL)
    INSTALL(FILES ${_out_file} DESTINATION ${SALOME_CONFIGURE_FILE_INSTALL})
  ENDIF(SALOME_CONFIGURE_FILE_INSTALL)
ENDMACRO(SALOME_CONFIGURE_FILE)


#######################################################################################
# Useful macros for SALOME own package detection system
#

###
# SALOME_CHECK_EQUAL_PATHS(result path1 path2)
#  Check if two paths are identical, resolving links. If the paths do not exist a simple
#  text comparison is performed.
#  result is a boolean.
###
MACRO(SALOME_CHECK_EQUAL_PATHS varRes path1 path2)  
  SET("${varRes}" OFF)
  IF(EXISTS "${path1}")
    GET_FILENAME_COMPONENT(_tmp1 "${path1}" REALPATH)
  ELSE()
    SET(_tmp1 "${path1}")
  ENDIF() 

  IF(EXISTS "${path2}")
    GET_FILENAME_COMPONENT(_tmp2 "${path2}" REALPATH)
  ELSE()
    SET(_tmp2 "${path2}")
  ENDIF() 

  IF("${_tmp1}" STREQUAL "${_tmp2}")
    SET("${varRes}" ON)
  ENDIF()
#  MESSAGE(${${varRes}})
ENDMACRO()

####
# SALOME_LOG_OPTIONAL_PACKAGE(pkg flag)
#
# Register in global variables the detection status (found or not) of the optional package 'pkg' 
# and the configuration flag that should be turned off to avoid detection of the package.
# The global variables are read again by SALOME_PACKAGE_REPORT_AND_CHECK to produce 
# a summary report of the detection status and stops the process if necessary.
MACRO(SALOME_LOG_OPTIONAL_PACKAGE pkg flag)
  # Was the package found
  STRING(TOUPPER ${pkg} _pkg_UC)
  IF(${pkg}_FOUND OR ${_pkg_UC}_FOUND)
    SET(_isFound TRUE)
  ELSE()
    SET(_isFound FALSE)
  ENDIF()

  # Is the package already in the list? Then update its status:
  LIST(FIND _SALOME_OPTIONAL_PACKAGES_names ${pkg} _result)
  IF(NOT ${_result} EQUAL -1)
    LIST(REMOVE_AT _SALOME_OPTIONAL_PACKAGES_found ${_result})
    LIST(REMOVE_AT _SALOME_OPTIONAL_PACKAGES_flags ${_result})
    LIST(INSERT    _SALOME_OPTIONAL_PACKAGES_found ${_result} ${_isFound})
    LIST(INSERT    _SALOME_OPTIONAL_PACKAGES_flags ${_result} ${flag})
  ELSE()
    # Otherwise insert it
    LIST(APPEND _SALOME_OPTIONAL_PACKAGES_names ${pkg})
    LIST(APPEND _SALOME_OPTIONAL_PACKAGES_found ${_isFound})
    LIST(APPEND _SALOME_OPTIONAL_PACKAGES_flags ${flag})
  ENDIF() 
  
ENDMACRO(SALOME_LOG_OPTIONAL_PACKAGE)

####
# SALOME_JUSTIFY_STRING()
#
# Justifies the string specified as an argument to the given length
# adding required number of spaces to the end. Does noting if input
# string is longer as required length.
# Puts the result to the output variable.
#
# USAGE: SALOME_JUSTIFY_STRING(input length result)
#
# ARGUMENTS:
#   input  [in] input string
#   length [in] required length of resulting string
#   result [out] name of variable where the result string is put to
#
MACRO(SALOME_JUSTIFY_STRING input length result)
  SET(${result} ${input})
  STRING(LENGTH ${input} _input_length)
  MATH(EXPR _nb_spaces "${length}-${_input_length}-1")
  IF (_nb_spaces GREATER 0)
    FOREACH(_idx RANGE ${_nb_spaces})  
      SET(${result} "${${result}} ")
    ENDFOREACH()
  ENDIF()
ENDMACRO(SALOME_JUSTIFY_STRING)

####
# SALOME_PACKAGE_REPORT_AND_CHECK()
#
# Print a quick summary of the detection of optional prerequisites.
# If a package was not found, the configuration is stopped. The summary also indicates 
# which flag should be turned off to skip the detection of the package. 
#
# If optional JUSTIFY argument is specified, names of packages
# are left-justified to the given length; default value is 10.
#
# USAGE: SALOME_PACKAGE_REPORT_AND_CHECK([JUSTIFY length])
#
MACRO(SALOME_PACKAGE_REPORT_AND_CHECK)
  SET(_will_fail OFF)
  PARSE_ARGUMENTS(SALOME_PACKAGE_REPORT "JUSTIFY" "" ${ARGN})
  IF(SALOME_PACKAGE_REPORT_JUSTIFY)
    SET(_length ${SALOME_PACKAGE_REPORT_JUSTIFY})
  ELSE()
    SET(_length 23)
  ENDIF()
  MESSAGE(STATUS "") 
  MESSAGE(STATUS "  Optional packages - Detection report ")
  MESSAGE(STATUS "  ==================================== ")
  MESSAGE(STATUS "")
  IF(DEFINED _SALOME_OPTIONAL_PACKAGES_names)
    LIST(LENGTH _SALOME_OPTIONAL_PACKAGES_names _list_len)
    # Another CMake stupidity - FOREACH(... RANGE r) generates r+1 numbers ...
    MATH(EXPR _range "${_list_len}-1")
    FOREACH(_idx RANGE ${_range})  
      LIST(GET _SALOME_OPTIONAL_PACKAGES_names ${_idx} _pkg_name)
      LIST(GET _SALOME_OPTIONAL_PACKAGES_found ${_idx} _pkg_found)
      LIST(GET _SALOME_OPTIONAL_PACKAGES_flags ${_idx} _pkg_flag)
      SALOME_JUSTIFY_STRING(${_pkg_name} ${_length} _pkg_name)
      IF(_pkg_found)
        SET(_found_msg "Found")
        SET(_flag_msg "")
      ELSE()
        SET(_will_fail ON)
        SET(_found_msg "NOT Found")
        SET(_flag_msg " - ${_pkg_flag} can be switched OFF to skip this prerequisite.")
      ENDIF()
    
      MESSAGE(STATUS "  * ${_pkg_name}  ->  ${_found_msg}${_flag_msg}")
    ENDFOREACH()
  ENDIF(DEFINED _SALOME_OPTIONAL_PACKAGES_names)
  MESSAGE(STATUS "")
  MESSAGE(STATUS "")
  
  # Failure if some packages were missing:
  IF(_will_fail)
    MESSAGE(FATAL_ERROR "Some required prerequisites have NOT been found. Take a look at the report above to fix this.")
  ENDIF()
ENDMACRO(SALOME_PACKAGE_REPORT_AND_CHECK)

####
# SALOME_FIND_PACKAGE(englobingPackageName standardPackageName modus [onlyTryQuietly])
#
# example:  SALOME_FIND_PACKAGE(SalomeVTK VTK CONFIG)
#
# Encapsulate the call to the standard FIND_PACKAGE(standardPackageName) passing all the options
# given when calling the command FIND_PACKAGE(SalomeXYZ). Those options are stored implicitly in 
# CMake variables: xyz__FIND_QUIETLY, xyz_FIND_REQUIRED, etc ...
# 
# If a list of components was specified when invoking the initial FIND_PACKAGE(SalomeXyz ...) this is 
# also handled properly.
#
# Modus is either MODULE or CONFIG (cf standard FIND_PACKAGE() documentation).
# The last argument is optional and if set to TRUE will force the search to be OPTIONAL and QUIET.
# If the package is looked for in CONFIG mode, the standard system paths are skipped. If you still want a 
# system installation to be found in this mode, you have to set the ROOT_DIR variable explicitly to /usr (for
# example). 
#  
# This macro is to be called from within the FindSalomeXXXX.cmake file.
#
####
MACRO(SALOME_FIND_PACKAGE englobPkg stdPkg mode)
  SET(_OPT_ARG ${ARGV3})
  # Only bother if the package was not already found:
  # Some old packages use the lower case version - standard should be to always use
  # upper case:
  STRING(TOUPPER ${stdPkg} stdPkgUC)
  IF(NOT (${stdPkg}_FOUND OR ${stdPkgUC}_FOUND))
    IF(${englobPkg}_FIND_QUIETLY OR _OPT_ARG)
      SET(_tmp_quiet "QUIET")
    ELSE()
      SET(_tmp_quiet)
    ENDIF()  
    IF(${englobPkg}_FIND_REQUIRED AND NOT _OPT_ARG)
      SET(_tmp_req "REQUIRED")
    ELSE()
      SET(_tmp_req)
    ENDIF()  
    IF(${englobPkg}_FIND_VERSION_EXACT)
      SET(_tmp_exact "EXACT")
    ELSE()
      SET(_tmp_exact)
    ENDIF()

    # Call the CMake FIND_PACKAGE() command:    
    STRING(TOLOWER ${stdPkg} _pkg_lc)
    IF(("${mode}" STREQUAL "NO_MODULE") OR ("${mode}" STREQUAL "CONFIG"))
      # Hope to find direclty a CMake config file, indicating the SALOME CMake file
      # paths (the command already looks in places like "share/cmake", etc ... by default)
      # Note the options NO_CMAKE_BUILDS_PATH, NO_CMAKE_PACKAGE_REGISTRY to avoid (under Windows)
      # looking into a previous CMake build done via a GUI, or into the Win registry.
      # NO_CMAKE_SYSTEM_PATH and NO_SYSTEM_ENVIRONMENT_PATH ensure any _system_ files like 'xyz-config.cmake' 
      # don't get loaded (typically Boost). To force their loading, set the XYZ_ROOT_DIR variable to '/usr'. 
      # See documentation of FIND_PACKAGE() for full details.
      
      # Do we need to call the signature using components?
      IF(${englobPkg}_FIND_COMPONENTS)
        FIND_PACKAGE(${stdPkg} ${${englobPkg}_FIND_VERSION} ${_tmp_exact} 
              NO_MODULE ${_tmp_quiet} ${_tmp_req} COMPONENTS ${${englobPkg}_FIND_COMPONENTS}
              PATH_SUFFIXES "salome_adm/cmake_files" "adm_local/cmake_files"
              NO_CMAKE_BUILDS_PATH NO_CMAKE_PACKAGE_REGISTRY NO_CMAKE_SYSTEM_PACKAGE_REGISTRY NO_CMAKE_SYSTEM_PATH
                NO_SYSTEM_ENVIRONMENT_PATH)
      ELSE()
        FIND_PACKAGE(${stdPkg} ${${englobPkg}_FIND_VERSION} ${_tmp_exact} 
              NO_MODULE ${_tmp_quiet} ${_tmp_req}
              PATH_SUFFIXES "salome_adm/cmake_files" "adm_local/cmake_files"
              NO_CMAKE_BUILDS_PATH NO_CMAKE_PACKAGE_REGISTRY NO_CMAKE_SYSTEM_PACKAGE_REGISTRY NO_CMAKE_SYSTEM_PATH
                 NO_SYSTEM_ENVIRONMENT_PATH)
      ENDIF()
      MARK_AS_ADVANCED(${stdPkg}_DIR)
      
    ELSEIF("${mode}" STREQUAL "MODULE")
    
      # Do we need to call the signature using components?
      IF(${englobPkg}_FIND_COMPONENTS)
        FIND_PACKAGE(${stdPkg} ${${englobPkg}_FIND_VERSION} ${_tmp_exact} 
              MODULE ${_tmp_quiet} ${_tmp_req} COMPONENTS ${${englobPkg}_FIND_COMPONENTS})
      ELSE()
        FIND_PACKAGE(${stdPkg} ${${englobPkg}_FIND_VERSION} ${_tmp_exact} 
              MODULE ${_tmp_quiet} ${_tmp_req})
      ENDIF()
      
    ELSE()
    
      MESSAGE(FATAL_ERROR "Invalid mode argument in the call to the macro SALOME_FIND_PACKAGE. Should be CONFIG or MODULE.")
      
    ENDIF()
    
  ENDIF()
ENDMACRO()


####################################################################
# SALOME_FIND_PACKAGE_DETECT_CONFLICTS(pkg referenceVariable upCount)
#    pkg              : name of the system package to be detected
#    referenceVariable: variable containing a path that can be browsed up to 
# retrieve the package root directory (xxx_ROOT_DIR)
#    upCount          : number of times we have to go up from the path <referenceVariable>
# to obtain the package root directory.
#   
# For example:  SALOME_FIND_PACKAGE_DETECT_CONFLICTS(SWIG SWIG_EXECUTABLE 2) 
#
# Generic detection (and conflict check) procedure for package XYZ:
# 1. Load a potential env variable XYZ_ROOT_DIR as a default choice for the cache entry XYZ_ROOT_DIR
#    If empty, load a potential XYZ_ROOT_DIR_EXP as default value (path exposed by another package depending
# directly on XYZ)
# 2. Invoke FIND_PACKAGE() in this order:
#    * in CONFIG mode first (if possible): priority is given to a potential 
#    "XYZ-config.cmake" file
#    * then switch to the standard MODULE mode, appending on CMAKE_PREFIX_PATH 
# the above XYZ_ROOT_DIR variable
# 3. Extract the path actually found into a temp variable _XYZ_TMP_DIR
# 4. Warn if XYZ_ROOT_DIR is set and doesn't match what was found (e.g. when CMake found the system installation
#    instead of what is pointed to by XYZ_ROOT_DIR - happens when a typo in the content of XYZ_ROOT_DIR).
# 5. Conflict detection:
#    * check the temp variable against a potentially existing XYZ_ROOT_DIR_EXP
# 6. Finally expose what was *actually* found in XYZ_ROOT_DIR.  
# 7. Specific stuff: for example exposing a prerequisite of XYZ to the rest of the world for future 
# conflict detection. This is added after the call to the macro by the callee.
#
MACRO(SALOME_FIND_PACKAGE_AND_DETECT_CONFLICTS pkg referenceVariable upCount)
  ##
  ## 0. Initialization
  ##
  
  # Package name, upper case
  STRING(TOUPPER ${pkg} pkg_UC)

  ##
  ## 1. Load environment or any previously detected root dir for the package
  ##
  IF(DEFINED ENV{${pkg_UC}_ROOT_DIR})
    FILE(TO_CMAKE_PATH "$ENV{${pkg_UC}_ROOT_DIR}" _${pkg_UC}_ROOT_DIR_ENV)
    SET(_dflt_value "${_${pkg_UC}_ROOT_DIR_ENV}")
  ELSE()
    # will be blank if no package was previously loaded:
    SET(_dflt_value "${${pkg_UC}_ROOT_DIR_EXP}")
  ENDIF()

  # Detect if the variable has been set on the command line or elsewhere:
  IF(DEFINED ${pkg_UC}_ROOT_DIR)
     SET(_var_already_there TRUE)
  ELSE()
     SET(_var_already_there FALSE)
  ENDIF()
  #   Make cache entry 
  SET(${pkg_UC}_ROOT_DIR "${_dflt_value}" CACHE PATH "Path to ${pkg_UC} directory")

  ##
  ## 2. Find package - try CONFIG mode first (i.e. looking for XYZ-config.cmake)
  ##
  
  # Override the variable - don't append to it, as it would give precedence
  # to what was stored there before!  
  SET(CMAKE_PREFIX_PATH "${${pkg_UC}_ROOT_DIR}")
    
  # Try find_package in config mode. This has the priority, but is 
  # performed QUIET and not REQUIRED:
  SALOME_FIND_PACKAGE("Salome${pkg}" ${pkg} NO_MODULE TRUE)
  
  IF (${pkg_UC}_FOUND OR ${pkg}_FOUND)
    MESSAGE(STATUS "Found ${pkg} in CONFIG mode!")
  ENDIF()

  # Otherwise try the standard way (module mode, with the standard CMake Find*** macro):
  # We do it quietly to produce our own error message, except if we are in debug mode:
  IF(SALOME_CMAKE_DEBUG)
    SALOME_FIND_PACKAGE("Salome${pkg}" ${pkg} MODULE FALSE)
  ELSE()
    SALOME_FIND_PACKAGE("Salome${pkg}" ${pkg} MODULE TRUE)
  ENDIF()
  
  # Set the "FOUND" variable for the SALOME wrapper:
  IF(${pkg_UC}_FOUND OR ${pkg}_FOUND)
    SET(SALOME${pkg_UC}_FOUND TRUE)
  ELSE()
    SET(SALOME${pkg_UC}_FOUND FALSE)
    IF(NOT Salome${pkg}_FIND_QUIETLY)
      IF(Salome${pkg}_FIND_REQUIRED)
         MESSAGE(FATAL_ERROR "Package ${pkg} couldn't be found - did you set the corresponing root dir correctly? "
         "It currently contains ${pkg_UC}_ROOT_DIR=${${pkg_UC}_ROOT_DIR}  "
         "Append -DSALOME_CMAKE_DEBUG=ON on the command line if you want to see the original CMake error.")
      ELSE()
         MESSAGE(WARNING "Package ${pkg} couldn't be found - did you set the corresponing root dir correctly? "
         "It currently contains ${pkg_UC}_ROOT_DIR=${${pkg_UC}_ROOT_DIR}  "
         "Append -DSALOME_CMAKE_DEBUG=ON on the command line if you want to see the original CMake error.")
      ENDIF()
    ENDIF()
  ENDIF()
  
  IF (${pkg_UC}_FOUND OR ${pkg}_FOUND)
    ## 3. Set the root dir which was finally retained by going up "upDir" times
    ## from the given reference path. The variable "referenceVariable" may be a list.
    ## In this case we take its first element. 
    
    # First test if the variable exists, warn otherwise:
    IF(NOT DEFINED ${referenceVariable})
      MESSAGE(WARNING "${pkg}: the reference variable '${referenceVariable}' used when calling the macro "
      "SALOME_FIND_PACKAGE_AND_DETECT_CONFLICTS() is not defined.")
    ENDIF()
    
    LIST(LENGTH ${referenceVariable} _tmp_len)
    IF(_tmp_len)
       LIST(GET ${referenceVariable} 0 _tmp_ROOT_DIR)
    ELSE()
       #  Note the double de-reference of "referenceVariable":
       SET(_tmp_ROOT_DIR "${${referenceVariable}}")
    ENDIF()
    IF(${upCount}) 
      FOREACH(_unused RANGE 1 ${upCount})        
        GET_FILENAME_COMPONENT(_tmp_ROOT_DIR "${_tmp_ROOT_DIR}" PATH)
      ENDFOREACH()
    ENDIF()

    ##
    ## 4. Warn if CMake found something not located under ENV(XYZ_ROOT_DIR)
    ##
    IF(DEFINED ENV{${pkg_UC}_ROOT_DIR})
      SALOME_CHECK_EQUAL_PATHS(_res "${_tmp_ROOT_DIR}" "${_${pkg_UC}_ROOT_DIR_ENV}")
      IF(NOT _res)
        MESSAGE(WARNING "${pkg} was found, but not at the path given by the "
            "environment ${pkg_UC}_ROOT_DIR! Is the variable correctly set? "
            "The two paths are: ${_tmp_ROOT_DIR} and: ${_${pkg_UC}_ROOT_DIR_ENV}")
        
      ELSE()
        MESSAGE(STATUS "${pkg} found directory matches what was specified in the ${pkg_UC}_ROOT_DIR variable, all good!")    
      ENDIF()
    ELSE()
        IF(NOT _var_already_there) 
          MESSAGE(STATUS "Variable ${pkg_UC}_ROOT_DIR was not explicitly defined. "
          "An installation was found anyway: ${_tmp_ROOT_DIR}")
        ENDIF()
    ENDIF()

    ##
    ## 5. Conflict detection
    ##     From another prerequisite using the package:
    ##
    IF(${pkg_UC}_ROOT_DIR_EXP)
        SALOME_CHECK_EQUAL_PATHS(_res "${_tmp_ROOT_DIR}" "${${pkg_UC}_ROOT_DIR_EXP}") 
        IF(NOT _res)
           MESSAGE(WARNING "Warning: ${pkg}: detected version conflicts with a previously found ${pkg}!"
                           " The two paths are " ${_tmp_ROOT_DIR} " vs " ${${pkg_UC}_ROOT_DIR_EXP})
        ELSE()
            MESSAGE(STATUS "${pkg} directory matches what was previously exposed by another prereq, all good!")
        ENDIF()        
    ENDIF()
    
    ##
    ## 6. Save the detected installation
    ##
    SET(${pkg_UC}_ROOT_DIR "${_tmp_ROOT_DIR}")
     
  ELSE()
    MESSAGE(STATUS "${pkg} was not found.")  
  ENDIF()
  
  SET(Salome${pkg}_FOUND "${pkg}_FOUND")
ENDMACRO(SALOME_FIND_PACKAGE_AND_DETECT_CONFLICTS)


####################################################################
# SALOME_ADD_MPI_TO_HDF5()
# 
# Overload the HDF5 flags so that they also contain MPI references.
# This is to be used when HDF5 was compiled with MPI support;
MACRO(SALOME_ADD_MPI_TO_HDF5)  
  SET(HDF5_INCLUDE_DIRS ${HDF5_INCLUDE_DIRS} ${MPI_INCLUDE_DIRS})
  SET(HDF5_DEFINITIONS "${HDF5_DEFINITIONS} ${MPI_DEFINITIONS}")
  SET(HDF5_LIBRARIES ${HDF5_LIBRARIES} ${MPI_LIBRARIES})
ENDMACRO(SALOME_ADD_MPI_TO_HDF5)

####################################################################
# SALOME_XVERSION()
# 
# Computes hexadecimal version of SALOME package
#
# USAGE: SALOME_XVERSION(package)
#
# ARGUMENTS:
#
# package: IN: SALOME package name
#
# The macro reads SALOME package version from PACKAGE_VERSION variable
# (note package name in uppercase as assumed for SALOME modules);
# hexadecimal version value in form 0xAABBCC (where AA, BB and CC are
# major, minor and maintenance components of package version in
# hexadecimal form) is put to the PACKAGE_XVERSION variable
MACRO(SALOME_XVERSION pkg)
  STRING(TOUPPER ${pkg} _pkg_UC)
  IF(${_pkg_UC}_VERSION)
    EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} -c "import sys; t=sys.argv[-1].split(\".\") ; t[:]=(int(elt) for elt in t) ; sys.stdout.write(\"0x%02x%02x%02x\"%tuple(t))" ${${_pkg_UC}_VERSION}
                    OUTPUT_VARIABLE ${_pkg_UC}_XVERSION)
  ENDIF()
ENDMACRO(SALOME_XVERSION)

#########################################################################
# SALOME_ACCUMULATE_HEADERS()
# 
# This macro is called in the various FindSalomeXYZ.cmake modules to accumulate
# internally the list of include headers to be saved for future export. 
# The full set of include is saved in a variable called 
#      _${PROJECT_NAME}_EXTRA_HEADERS
#
MACRO(SALOME_ACCUMULATE_HEADERS lst)
  FOREACH(l IN LISTS ${lst})
    LIST(FIND _${PROJECT_NAME}_EXTRA_HEADERS "${l}" _res)
    IF(_res EQUAL "-1")
      IF(NOT "${l}" STREQUAL "/usr/include")
        LIST(APPEND _${PROJECT_NAME}_EXTRA_HEADERS "${l}")
      ENDIF()
    ENDIF()
  ENDFOREACH()
ENDMACRO(SALOME_ACCUMULATE_HEADERS)

#########################################################################
# SALOME_ACCUMULATE_ENVIRONMENT()
# 
# USAGE: SALOME_ACCUMULATE_ENVIRONMENT(envvar value [value ...])
#
# ARGUMENTS:
#   envvar [in] environment variable name, e.g. PATH
#   value  [in] value(s) to be added to environment variable
#
# This macro is called in the various FindSalomeXYZ.cmake modules to 
# accumulate environment variables, to be used later to run some command
# in proper environment.
#
# 1. Each envrironment variable is stored in specific CMake variable
#    _${PROJECT_NAME}_EXTRA_ENV_<var>, where <var> is name of variable.
# 2. Full list of environment variable names is stored in CMake variable
#    _${PROJECT_NAME}_EXTRA_ENV.
#
MACRO(SALOME_ACCUMULATE_ENVIRONMENT envvar)
  FOREACH(_item ${ARGN})
    IF(EXISTS ${_item})
      IF(IS_DIRECTORY ${_item})
        LIST(FIND _${PROJECT_NAME}_EXTRA_ENV_${envvar} ${_item} _res)
        IF(_res EQUAL -1)
          LIST(APPEND _${PROJECT_NAME}_EXTRA_ENV_${envvar} ${_item})
        ENDIF()
      ELSE()
        GET_FILENAME_COMPONENT(_path_dir ${_item} PATH)
        LIST(FIND _${PROJECT_NAME}_EXTRA_ENV_${envvar} ${_path_dir} _res)
        IF(_res EQUAL -1)
          LIST(APPEND _${PROJECT_NAME}_EXTRA_ENV_${envvar} ${_path_dir})
        ENDIF()
      ENDIF()
    ENDIF()
  ENDFOREACH()
  LIST(FIND _${PROJECT_NAME}_EXTRA_ENV ${envvar} _res)
  IF(_res EQUAL -1)
    LIST(APPEND _${PROJECT_NAME}_EXTRA_ENV ${envvar})
  ENDIF()
  SET(_${PROJECT_NAME}_EXTRA_ENV_FULL "SET\(${PROJECT_NAME}_EXTRA_ENV ${_${PROJECT_NAME}_EXTRA_ENV}\)")
  FOREACH(_res ${_${PROJECT_NAME}_EXTRA_ENV})
    SET(_${PROJECT_NAME}_EXTRA_ENV_FULL "${_${PROJECT_NAME}_EXTRA_ENV_FULL}\nSET\(${PROJECT_NAME}_EXTRA_ENV_${_res} ${_${PROJECT_NAME}_EXTRA_ENV_${_res}}\)")
  ENDFOREACH()
ENDMACRO(SALOME_ACCUMULATE_ENVIRONMENT)

#########################################################################
# SALOME_GENERATE_ENVIRONMENT_SCRIPT()
# 
# USAGE: SALOME_GENERATE_ENVIRONMENT_SCRIPT(output script cmd opts)
#
# ARGUMENTS:
#   output [out] output command, e.g. for creation of target.
#   script [in]  output environement script name
#   cmd    [in]  input command, e.g. sphinx or python command.
#   opts   [in]  options for input command (cmd).
#
# This macro is called when it's necessary to use given environment to run some command. 
# Macro generates environement script using previously created variables
# _${PROJECT_NAME}_EXTRA_ENV_<var>, where <var> is name of variable and
# _${PROJECT_NAME}_EXTRA_ENV (see marco SALOME_ACCUMULATE_ENVIRONMENT);
# and puts generated command in proper environment into <output> argument.
# 
# Notes:
# - If <script> is specified as relative path, it is computed from the current build
#   directory.
#
MACRO(SALOME_GENERATE_ENVIRONMENT_SCRIPT output script cmd opts)
  IF(IS_ABSOLUTE ${script})
    SET(_script ${script})
  ELSE()
    SET(_script ${CMAKE_CURRENT_BINARY_DIR}/${script})
  ENDIF()

  SET(_env)
  FOREACH(_item ${_${PROJECT_NAME}_EXTRA_ENV})
    FOREACH(_val ${_${PROJECT_NAME}_EXTRA_ENV_${_item}})
      IF(WIN32)
        IF(${_item} STREQUAL "LD_LIBRARY_PATH")
          SET(_item PATH)
        ENDIF()
        STRING(REPLACE "/" "\\" _env "${_env} @SET ${_item}=${_val}\;%${_item}%\n")
        SET(_ext "bat")
        SET(_call_cmd "call")
      ELSE(WIN32)
        SET(_env "${_env} export ${_item}=${_val}:\${${_item}}\n")
        SET(_ext "sh")
        SET(_call_cmd ".")
      ENDIF(WIN32)
    ENDFOREACH()
  ENDFOREACH()
  
  SET(_script ${_script}.${_ext})
  FILE(WRITE ${_script} "${_env}")
  
  SET(${output} ${_call_cmd} ${_script} && ${cmd} ${opts})
  
ENDMACRO(SALOME_GENERATE_ENVIRONMENT_SCRIPT)

#########################################################################
# SALOME_APPEND_LIST_OF_LIST()
# 
# USAGE: SALOME_APPEND_LIST_OF_LIST(result element_list)
#
# Build a list of lists. The element_list is first parsed to convert it 
# from 
#     a;b;c;d;e
# to 
#     a,b,c,d,e
#
# It is then added to the big list 'result'. Hence 'result' looks like:
#     a,b,c,d,e;f,g,h; ...
#
MACRO(SALOME_APPEND_LIST_OF_LIST result element_list)
  SET(_tmp_res)
  STRING(REPLACE ";" "," _tmp_res "${${element_list}}")
  LIST(APPEND ${result} ${_tmp_res})
ENDMACRO(SALOME_APPEND_LIST_OF_LIST)

#########################################################################
# SALOME_CONFIGURE_PREPARE()
# 
# USAGE: SALOME_CONFIGURE_PREPARE(pkg1 pkg2 ...)
#
# Prepare the variable that will be used to configure the file Salome<MODULE>Config.cmake,
# namely:
#    - _PREREQ_LIST      : the list of level 1 external prerequisites
#    - _PREREQ_DIR_LIST  : their corresponding CMake directories (i.e. where the CMake configuration
#    file for this package can be found, if there is any!)
#    - _PREREQ_COMPO_LIST: the list of components requested when this package was invoked
#
# All this information is built from the package_list, the list of level 1 packages for this module.
# Only the packages found in CONFIG mode are retained.
#
MACRO(SALOME_CONFIGURE_PREPARE)
  SET(_tmp_prereq "${ARGV}")
  SET(_PREREQ_LIST)
  SET(_PREREQ_DIR_LIST)
  SET(_PREREQ_COMPO_LIST)
  FOREACH(_prereq IN LISTS _tmp_prereq)
    IF(${_prereq}_DIR)
      SET(_PREREQ_LIST "${_PREREQ_LIST} ${_prereq}")
      SET(_PREREQ_DIR_LIST "${_PREREQ_DIR_LIST} \"${${_prereq}_DIR}\"")
      SALOME_APPEND_LIST_OF_LIST(_PREREQ_COMPO_LIST Salome${_prereq}_COMPONENTS)
    ENDIF()
  ENDFOREACH()
ENDMACRO(SALOME_CONFIGURE_PREPARE)