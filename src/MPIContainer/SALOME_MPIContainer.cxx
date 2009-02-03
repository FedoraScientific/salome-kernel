//  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
#include <mpi.h>
#include <iostream>
#include "MPIContainer_i.hxx"
#include "Utils_ORB_INIT.hxx"
#include "Utils_SINGLETON.hxx"
#include "utilities.h"
using namespace std;

int main(int argc, char* argv[])
{
  int nbproc, numproc;
  Engines_MPIContainer_i * myContainer=NULL;

  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD,&nbproc);
  MPI_Comm_rank(MPI_COMM_WORLD,&numproc);

  // Initialise the ORB.
  ORB_INIT &init = *SINGLETON_<ORB_INIT>::Instance() ;
  CORBA::ORB_var &orb = init( argc , argv ) ;
  //  SALOMETraceCollector *myThreadTrace = SALOMETraceCollector::instance(orb);
 
  BEGIN_OF("[" << numproc << "] " << argv[0])
  try {
    
    // Obtain a reference to the root POA.
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var root_poa = PortableServer::POA::_narrow(obj);

    // obtain the root poa manager
    PortableServer::POAManager_var pman = root_poa->the_POAManager();

    // define policy objects     
    PortableServer::ImplicitActivationPolicy_var implicitActivation =
      root_poa->create_implicit_activation_policy(PortableServer::NO_IMPLICIT_ACTIVATION) ;

      // default = NO_IMPLICIT_ACTIVATION
    PortableServer::ThreadPolicy_var threadPolicy =
      root_poa->create_thread_policy(PortableServer::ORB_CTRL_MODEL) ;
      // default = ORB_CTRL_MODEL, other choice SINGLE_THREAD_MODEL

    // create policy list
    CORBA::PolicyList policyList;
    policyList.length(2);
    policyList[0] = PortableServer::ImplicitActivationPolicy::_duplicate(implicitActivation) ;
    policyList[1] = PortableServer::ThreadPolicy::_duplicate(threadPolicy) ;

    // create the child POA
    PortableServer::POAManager_var nil_mgr = PortableServer::POAManager::_nil() ;
    PortableServer::POA_var factory_poa =
      root_poa->create_POA("factory_poa", pman, policyList) ;
      //with nil_mgr instead of pman, a new POA manager is created with the new POA
    
    // destroy policy objects
    implicitActivation->destroy() ;
    threadPolicy->destroy() ;

    char *containerName = "";
    if (argc >1) 
    {
	containerName = argv[1] ;
    }

    MESSAGE("[" << numproc << "] MPIContainer: load MPIContainer servant");
    myContainer = new Engines_MPIContainer_i(nbproc,numproc,orb,factory_poa, containerName,argc,argv);

    pman->activate();

    orb->run();

  }
  catch(CORBA::SystemException&){
    INFOS("Caught CORBA::SystemException.");
  }
  catch(PortableServer::POA::WrongPolicy&){
    INFOS("Caught CORBA::WrongPolicyException.");
  }
  catch(PortableServer::POA::ServantAlreadyActive&){
    INFOS("Caught CORBA::ServantAlreadyActiveException");
  }
  catch(CORBA::Exception&){
    INFOS("Caught CORBA::Exception.");
  }
  catch(...){
    INFOS("Caught unknown exception.");
  }

  if(myContainer)
    delete myContainer;

  END_OF("[" << numproc << "] " << argv[0]);
  //  delete myThreadTrace;

  MPI_Finalize();

}

