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
#ifndef __SALOME_CONTAINERMANAGER_HXX__
#define __SALOME_CONTAINERMANAGER_HXX__

#include <SALOMEconfig.h>
#include CORBA_CLIENT_HEADER(SALOME_Component)
#include CORBA_CLIENT_HEADER(SALOME_ContainerManager)
#include "SALOME_ResourcesManager.hxx"
#include "SALOME_LoadRateManager.hxx"

#include <string>

class SALOME_NamingService;

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

class CONTAINER_EXPORT SALOME_ContainerManager:
  public POA_Engines::ContainerManager,
  public PortableServer::RefCountServantBase
{

public:
  SALOME_ContainerManager(CORBA::ORB_ptr orb);
  ~SALOME_ContainerManager();

  Engines::Container_ptr
  FindOrStartContainer(const Engines::MachineParameters& params,
		       const Engines::MachineList& possibleComputer);

  Engines::MachineList *
  GetFittingResources(const Engines::MachineParameters& params,
		      const char *componentName);

  char* FindBest(const Engines::MachineList& possibleComputers);

  void Shutdown();
  void ShutdownContainers();

  static const char *_ContainerManagerNameInNS;

protected:
  Engines::Container_ptr
  FindContainer(const Engines::MachineParameters& params,
		const Engines::MachineList& possibleComputers);

  Engines::Container_ptr
  FindContainer(const Engines::MachineParameters& params,
		const char *theMachine);

  long GetIdForContainer(void);
  long _id;

  SALOME_ResourcesManager *_ResManager;
  SALOME_NamingService *_NS;
};

#endif
