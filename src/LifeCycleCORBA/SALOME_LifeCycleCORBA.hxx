//  SALOME LifeCycleCORBA : implementation of containers and engines life cycle both in Python and C++
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
//  File   : SALOME_LifeCycleCORBA.hxx
//  Author : Paul RASCLE, EDF - MARC TAJCHMAN, CEA
//  Module : SALOME
//  $Header$

#ifndef _SALOME_LIFECYCLECORBA_HXX_
#define _SALOME_LIFECYCLECORBA_HXX_

#include <stdlib.h>
#ifndef WNT
#include <unistd.h>
#endif
#include <string>

#include <SALOMEconfig.h>
#include <Utils_SALOME_Exception.hxx>
#include CORBA_CLIENT_HEADER(SALOME_ContainerManager)
#include CORBA_CLIENT_HEADER(SALOME_Component)

#if defined LIFECYCLECORBA_EXPORTS
#if defined WIN32
#define LIFECYCLECORBA_EXPORT __declspec( dllexport )
#else
#define LIFECYCLECORBA_EXPORT
#endif
#else
#if defined WNT
#define LIFECYCLECORBA_EXPORT __declspec( dllimport )
#else
#define LIFECYCLECORBA_EXPORT
#endif
#endif


class SALOME_NamingService;

class LIFECYCLECORBA_EXPORT IncompatibleComponent : public SALOME_Exception
{
public :
  IncompatibleComponent(void);
  IncompatibleComponent(const IncompatibleComponent &ex);
};

class LIFECYCLECORBA_EXPORT SALOME_LifeCycleCORBA
{
public:
  SALOME_LifeCycleCORBA(SALOME_NamingService *ns = 0);
  virtual ~SALOME_LifeCycleCORBA();

  Engines::Component_ptr 
  FindComponent(const Engines::MachineParameters& params,
		const char *componentName,
		int studyId=0);

  Engines::Component_ptr
  LoadComponent(const Engines::MachineParameters& params,
		const char *componentName,
		int studyId=0);

  Engines::Component_ptr 
  FindOrLoad_Component(const Engines::MachineParameters& params,
		       const char *componentName,
		       int studyId =0);

  Engines::Component_ptr
  FindOrLoad_Component(const char *containerName,
		       const char *componentName); // for compatibility
  
  bool isKnownComponentClass(const char *componentName);

  bool isMpiContainer(const Engines::MachineParameters& params)
    throw(IncompatibleComponent);

  int NbProc(const Engines::MachineParameters& params);

  void preSet(Engines::MachineParameters& params);

  Engines::ContainerManager_ptr getContainerManager();

protected:

  /*! Establish if a component called "componentName" in a container called
   *  "containerName"
   *  exists among the list of resources in "listOfMachines".
   *  This method uses Naming Service to find the component.
   */
  Engines::Component_ptr 
  _FindComponent(const Engines::MachineParameters& params,
		 const char *componentName,
		 int studyId,
		 const Engines::MachineList& listOfMachines);

  Engines::Component_ptr
  _LoadComponent(const Engines::MachineParameters& params,
		 const char *componentName,
		 int studyId,
		 const Engines::MachineList& listOfMachines);
  
  SALOME_NamingService *_NS;
  Engines::ContainerManager_var _ContManager;
  
} ;

#endif
