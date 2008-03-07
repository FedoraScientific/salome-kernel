// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public  
// License along with this library; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//


// ----------------------------------------------------------------------------

%module libSALOME_LifeCycleCORBA

%include <std_except.i>


// ----------------------------------------------------------------------------

%{
#include "utilities.h"
#include "SALOME_LifeCycleCORBA.hxx"
#include "SALOME_FileTransferCORBA.hxx"
#include "SALOME_NamingService.hxx"
#include "ServiceUnreachable.hxx"
#include "Utils_SALOME_Exception.hxx"

  using namespace std;

//--- from omniORBpy.h (not present on Debian Sarge packages)

struct omniORBpyAPI {

  PyObject* (*cxxObjRefToPyObjRef)(const CORBA::Object_ptr cxx_obj,
				   CORBA::Boolean hold_lock);
  // Convert a C++ object reference to a Python object reference.
  // If <hold_lock> is true, caller holds the Python interpreter lock.

  CORBA::Object_ptr (*pyObjRefToCxxObjRef)(PyObject* py_obj,
					   CORBA::Boolean hold_lock);
  // Convert a Python object reference to a C++ object reference.
  // Raises BAD_PARAM if the Python object is not an object reference.
  // If <hold_lock> is true, caller holds the Python interpreter lock.


  omniORBpyAPI();
  // Constructor for the singleton. Sets up the function pointers.
};

  omniORBpyAPI* api;

%}


// ----------------------------------------------------------------------------


%init
%{
  // init section

  PyObject* omnipy = PyImport_ImportModule((char*)"_omnipy");
  if (!omnipy)
  {
    PyErr_SetString(PyExc_ImportError,
		    (char*)"Cannot import _omnipy");
    return;
  }
  PyObject* pyapi = PyObject_GetAttrString(omnipy, (char*)"API");
  api = (omniORBpyAPI*)PyCObject_AsVoidPtr(pyapi);
  Py_DECREF(pyapi);
%}



// ----------------------------------------------------------------------------

%typemap(python,out) Engines::Container_ptr, Engines::Component_ptr, Engines::fileRef_ptr
{
  MESSAGE("typemap out on CORBA object ptr");
  SCRUTE($1);
  $result = api->cxxObjRefToPyObjRef($1, 1);
  SCRUTE($result);
}

%typemap(python,in) Engines::fileRef_ptr aFileRef
{
  MESSAGE("typemap in on CORBA object ptr");
  try {
     CORBA::Object_ptr obj = api->pyObjRefToCxxObjRef($input,1);
     $1 = Engines::fileRef::_narrow(obj);
     SCRUTE($1);
  }
  catch (...) {
     PyErr_SetString(PyExc_RuntimeError, "not a valid CORBA object ptr");
  }
}


%typemap(python,out) std::string, 
		    string
{
  MESSAGE("typemap out on std::string");
  SCRUTE($1);
  $result = PyString_FromString($1.c_str());
}

%typemap(typecheck) const Engines::MachineParameters &,
                    Engines::MachineParameters const &
{
  $1 = PyDict_Check($input);
}

%typemap(typecheck) std::string, 
		    string
{
  $1 = PyString_Check($input);
}

%typemap(python,in) std::string, 
		    string
{
  MESSAGE("typemap in on std::string");
  std::string str;
  if (PyString_Check($input) == 1)
    {
      char* value = PyString_AsString($input);
      str = value;
      $1 = str;
    }
  else 
    {
       MESSAGE("Not a string");
       PyErr_SetString(PyExc_TypeError,"Must Be a Python string");
       return NULL;
    }
}


%typemap(python,in) const Engines::MachineParameters &
{
  //printf("typemap in on Engines::MachineParameters\n");
  MESSAGE("typemap in on Engines::MachineParameters");
  if (PyDict_Check($input) == 1)
    {
      Engines::MachineParameters *param = new Engines::MachineParameters ;
      param->container_name = CORBA::string_dup("");
      param->hostname = CORBA::string_dup("");
      param->OS = CORBA::string_dup("");
      param->mem_mb = 0;
      param->cpu_clock = 0;
      param->nb_proc_per_node = 0;
      param->nb_node = 0;
      param->isMPI = false;
      param->parallelLib = CORBA::string_dup("");
      param->nb_component_nodes = 0;
      PyObject *key, *value;
      int pos = 0;
      while (PyDict_Next($input, &pos, &key, &value))
	{
	  char* keystr = PyString_AsString(key);
	  printf("key: %s\n", keystr);
	  if (strcmp(keystr,"container_name")==0)
	    {
	      param->container_name = CORBA::string_dup(PyString_AsString(value));
	    }
	  else if (strcmp(keystr,"hostname")==0)
	    {
	      param->hostname = CORBA::string_dup(PyString_AsString(value));
	    }
	  else if (strcmp(keystr,"OS")==0)
	    {
	      param->OS = CORBA::string_dup(PyString_AsString(value));
	    }
	  else if (strcmp(keystr,"mem_mb")==0)
	    {
	      param->mem_mb = PyLong_AsLong(value);
	    }
	  else if (strcmp(keystr,"cpu_clock")==0)
	    {
	      param->cpu_clock = PyLong_AsLong(value);
	    }
	  else if (strcmp(keystr,"nb_proc_per_node")==0)
	    {
	      param->nb_proc_per_node = PyLong_AsLong(value);
	    }
	  else if (strcmp(keystr,"nb_node")==0)
	    {
	      param->nb_node = PyLong_AsLong(value);
	    }
	  else if (strcmp(keystr,"isMPI")==0)
	    {
	      param->isMPI = PyLong_AsLong(value);
	    }
	  else if (strcmp(keystr,"parallelLib")==0)
	    {
	      param->parallelLib = CORBA::string_dup(PyString_AsString(value));
	    }
	  else if (strcmp(keystr,"nb_component_nodes")==0)
	    {
	      param->nb_component_nodes = PyLong_AsLong(value);
	    }
	}
      $1 = param;
    }
  else 
    {
       MESSAGE("Not a dictionnary");
       PyErr_SetString(PyExc_TypeError,"Must Be a Python Dictionnary");
       return NULL;
    }
}


%typemap(python,freearg) const Engines::MachineParameters &
{
  MESSAGE("delete $1");
  delete $1;
}

// ----------------------------------------------------------------------------

%include <Utils_SALOME_Exception.hxx>

%exception {
    Py_BEGIN_ALLOW_THREADS
    try {
      $action
    }
    catch (ServiceUnreachable) {
       Py_BLOCK_THREADS
       PyErr_SetString(PyExc_RuntimeError,"Naming Service Unreacheable");
       return NULL;
    }
    catch (SALOME_Exception &e) {
       Py_BLOCK_THREADS
       PyErr_SetString(PyExc_RuntimeError,e.what());
       return NULL;
    }
    catch (SALOME::SALOME_Exception &e) {
       Py_BLOCK_THREADS
       PyErr_SetString(PyExc_RuntimeError,e.details.text);
       return NULL;
    }
    catch (...) {
       Py_BLOCK_THREADS
       PyErr_SetString(PyExc_RuntimeError, "unknown exception");
       return NULL;
    }
    Py_END_ALLOW_THREADS
}


%include <SALOME_LifeCycleCORBA.hxx>
%include <SALOME_FileTransferCORBA.hxx>

