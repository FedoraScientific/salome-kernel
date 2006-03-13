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
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : SALOME_Container.cxx
//  Author : Paul RASCLE, EDF - MARC TAJCHMAN, CEA
//  Module : SALOME
//  $Header$

#ifdef HAVE_MPI2
#include <mpi.h>
#endif

#include <iostream>
#include <strstream>
#include <string>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <dlfcn.h>

#ifndef WNT
#include <unistd.h>
#else
#include <process.h>
#endif
#include "SALOME_Container_i.hxx"
#include "utilities.h"
#include "Utils_ORB_INIT.hxx"
#include "Utils_SINGLETON.hxx"
#include "OpUtil.hxx"

#ifdef CHECKTIME
#include <Utils_Timer.hxx>
#endif

#include "Container_init_python.hxx"

using namespace std;

extern "C" void HandleServerSideSignals(CORBA::ORB_ptr theORB);

int main(int argc, char* argv[])
{
#ifdef HAVE_MPI2
  MPI_Init(&argc,&argv);
#endif

  // Initialise the ORB.
  //SRN: BugID: IPAL9541, it's necessary to set a size of one message to be at least 100Mb
  //CORBA::ORB_var orb = CORBA::ORB_init( argc , argv ) ;
  ORB_INIT &init = *SINGLETON_<ORB_INIT>::Instance() ;
  ASSERT(SINGLETON_<ORB_INIT>::IsAlreadyExisting());
  CORBA::ORB_ptr orb = init(0 , 0 ) ;
	  
  //  LocalTraceCollector *myThreadTrace = SALOMETraceCollector::instance(orb);
  INFOS_COMPILATION;
  BEGIN_OF(argv[0]);

  ASSERT(argc > 1);
  SCRUTE(argv[1]);
  bool isSupervContainer = false;
  if (strcmp(argv[1],"SuperVisionContainer") == 0) isSupervContainer = true;

  if (!isSupervContainer)
    {
      int _argc = 1;
      char* _argv[] = {""};
      KERNEL_PYTHON::init_python(argc,argv);
    }
  else
    {
      Py_Initialize() ;
      PySys_SetArgv( argc , argv ) ;
    }
    
  char *containerName = "";
  if(argc > 1)
    {
      containerName = argv[1] ;
    }

  try
    {  
      CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
      ASSERT(!CORBA::is_nil(obj));
      PortableServer::POA_var root_poa = PortableServer::POA::_narrow(obj);

      PortableServer::POAManager_var pman = root_poa->the_POAManager();

      // add new container to the kill list
#ifndef WNT
      ostrstream aCommand ;
      aCommand << "addToKillList.py " << getpid() << " SALOME_Container" << ends ;
      system(aCommand.str());
#endif
      
      Engines_Container_i * myContainer 
	= new Engines_Container_i(orb, root_poa, containerName , argc , argv );
      
      pman->activate();
      
#ifdef CHECKTIME
      Utils_Timer timer;
      timer.Start();
      timer.Stop();
      timer.ShowAbsolute();
#endif

      HandleServerSideSignals(orb);
      
    }
  catch(CORBA::SystemException&)
    {
      INFOS("Caught CORBA::SystemException.");
    }
  catch(PortableServer::POA::ServantAlreadyActive&)
    {
      INFOS("Caught CORBA::ServantAlreadyActiveException");
    }
  catch(CORBA::Exception&)
    {
      INFOS("Caught CORBA::Exception.");
    }
  catch(std::exception& exc)
    {
      INFOS("Caught std::exception - "<<exc.what()); 
    }
  catch(...)
    {
      INFOS("Caught unknown exception.");
    }

#ifdef HAVE_MPI2
  MPI_Finalize();
#endif

  //END_OF(argv[0]);
  //LocalTraceBufferPool* bp1 = LocalTraceBufferPool::instance();
  //bp1->deleteInstance(bp1);
  return 0 ;
}

