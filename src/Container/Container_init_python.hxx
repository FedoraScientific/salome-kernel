//  SALOME Container : implementation of container and engine for Kernel
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
// 
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public 
//  License as published by the Free Software Foundation; either 
//  version 2.1 of the License. 
// 
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free Software 
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//
//
//  File   : Container_init_python.hxx
//  Author : Paul RASCLE, EDF
//  Module : KERNEL
//  $Header$

#ifndef _CONTAINER_INIT_PYTHON_HXX_
#define _CONTAINER_INIT_PYTHON_HXX_

#include <pthread.h>  // must be before Python.h !
#include <Python.h>


// next two MACRO must be used together only once inside a block
// -------------------------------------------------------------
// protect a sequence of Python calls:
// - Python lock must be acquired for these calls
// - new Python thread state allows multi thread use of the sequence:
//    - Python may release the lock within the sequence, so multiple
//      thread execution of the sequence may occur.
//    - For that case, each sequence call must use a specific Python
//      thread state.
//    - There is no need of C Lock protection of the sequence.


#if defined CONTAINER_EXPORTS
#if defined WIN32
#define CONTAINER_EXPORT __declspec( dllexport )
#else
#define CONTAINER_EXPORT
#endif
#else
#if defined WNT
#define CONTAINER_EXPORT __declspec( dllimport )
#else
#define CONTAINER_EXPORT
#endif
#endif

#define Py_ACQUIRE_NEW_THREAD \
  PyEval_AcquireLock(); \
  PyThreadState *myTstate = PyThreadState_New(KERNEL_PYTHON::_interp); \
  PyThreadState *myoldTstate = PyThreadState_Swap(myTstate);

#define Py_RELEASE_NEW_THREAD \
  PyEval_ReleaseThread(myTstate); \
  PyThreadState_Delete(myTstate);

struct CONTAINER_EXPORT KERNEL_PYTHON
{
#ifdef WNT
  static PyThreadState *get_gtstate() { return KERNEL_PYTHON::_gtstate; }
  static PyObject *getsalome_shared_modules_module() { return KERNEL_PYTHON::salome_shared_modules_module; }
  static PyInterpreterState *get_interp() { return KERNEL_PYTHON::_interp; }
#endif
  static PyThreadState *_gtstate;
  static PyObject *salome_shared_modules_module;
  static PyInterpreterState *_interp;

  static void init_python(int argc, char **argv);

};

#endif
