//  SALOME ParallelContainerProxyDummy : Proxy of a PaCO++ object using Dummy
//
//  Copyright (C) 2007 OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  File   : SALOME_ParallelContainerProxyDummy.cxx
//  Author : Andr� Ribes, EDF
//  Module : SALOME PARALLEL

#include <iostream>
#include <string>
#include <stdio.h>

#ifndef WNT
#include <unistd.h>
#else
#include <process.h>
#endif

// PaCO++ include
//#include "SALOME_ComponentPaCO_Engines_Container_server.h"
#include "SALOME_ParallelContainerProxy_i.hxx"
#include <paco_omni.h>
#include <paco_dummy.h>

#include "SALOME_NamingService.hxx"

#include "utilities.h"
#include "Utils_ORB_INIT.hxx"
#include "Utils_SINGLETON.hxx"
#include "SALOMETraceCollector.hxx"
#include "OpUtil.hxx"

#ifdef DEBUG_PARALLEL
#include <signal.h>
using namespace std;

void handler(int t) {
  cerr << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
  cerr << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
  cerr << "SIGSEGV in :" << getpid() << endl;
  cerr << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
  cerr << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
  while (1) {}
}
#endif

int main(int argc, char* argv[])
{
  INFOS("Launching a parallel proxy container");

#ifdef DEBUG_PARALLEL
  signal(SIGSEGV, handler);
#endif
  // Initialise the ORB.
  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

  std::string containerName("");
  if(argc > 1) {
    containerName = argv[1];
  }

  try {  
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    ASSERT(!CORBA::is_nil(obj));
    PortableServer::POA_var root_poa = PortableServer::POA::_narrow(obj);
    PortableServer::POAManager_var pman = root_poa->the_POAManager();

#ifndef WNT
    // add this container to the kill list
    char aCommand[100];
    sprintf(aCommand, "addToKillList.py %d SALOME_ParallelContainerProxyDummy", getpid());
    system(aCommand);
#endif

    SALOME_NamingService * ns = new SALOME_NamingService(CORBA::ORB::_duplicate(orb));
//    Engines::Container_proxy_impl * proxy = 
//      new Engines::Container_proxy_impl(orb,
//					new paco_omni_fabrique());

    Container_proxy_impl_final * proxy = 
      new Container_proxy_impl_final(orb,
				     new paco_omni_fabrique());
    // PaCO++ code
    paco_fabrique_manager* pfm = paco_getFabriqueManager();
    pfm->register_com("dummy", new paco_dummy_fabrique());
    proxy->setLibCom("dummy", proxy);
    pfm->register_thread("omnithread", new paco_omni_fabrique());
    proxy->setLibThread("omnithread");
    // Topo of the parallel object
    PaCO::PacoTopology_t serveur_topo;
    serveur_topo.total = 1;
    proxy->setTopology(serveur_topo);

    PortableServer::ObjectId_var _id = root_poa->activate_object(proxy);
    obj = root_poa->id_to_reference(_id);

    // In the NamingService
    string hostname = GetHostname();
    Engines::Container_var pCont = Engines::Container::_narrow(obj);
    string _containerName = ns->BuildContainerNameForNS(containerName.c_str(),
							hostname.c_str());
    cerr << "---------" << _containerName << "----------" << endl;
    ns->Register(pCont, _containerName.c_str());
    pman->activate();
    orb->run();
  }
  catch (PaCO::PACO_Exception& e)
  {
    INFOS("Caught PaCO::PACO_Exception");
    std::cerr << e << std::endl;
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
  return 0 ;
}

