//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : PyInterp_PyQt.cxx
//  Author : Christian CAREMOLI, Paul RASCLE, EDF
//  Module : SALOME
//  $Header$

#include "PyInterp_PyQt.h"
#include "utilities.h"

using namespace std;


/*!
 * constructor : the main SALOME Python interpreter is used for PyQt GUI.
 * calls initialize method defined in base class, which calls virtual methods
 * initstate & initcontext redefined here
 */
PyInterp_PyQt::PyInterp_PyQt(): PyInterp_base()
{
}

PyInterp_PyQt::~PyInterp_PyQt()
{
}

void PyInterp_PyQt::initState()
{
  SCRUTE(PyInterp_base::_gtstate);
  _tstate=PyInterp_base::_gtstate;
  PyThreadState_Swap(_tstate);
  SCRUTE(_tstate);
}

void PyInterp_PyQt::initContext()
{
  _g = PyDict_New();          // create interpreter dictionnary context
  PyObject *bimod = PyImport_ImportModule("__builtin__");
  PyDict_SetItemString(_g, "__builtins__", bimod);
  Py_DECREF(bimod);
}

void PyInterp_PyQt::run(const char *command)
{
  MESSAGE("compile");
  PyLockWrapper aLock(_tstate);
  PyObject *code = Py_CompileString((char *)command,"PyGUI",Py_file_input);
  if(!code){
    // Une erreur s est produite en general SyntaxError
    PyErr_Print();
    return;
  }
  PyObject *r = PyEval_EvalCode(code,_g,_g);
  Py_DECREF(code);
  if(!r){
    // Une erreur s est produite a l execution
    PyErr_Print();
    return;
  }
  Py_DECREF(r);
}

