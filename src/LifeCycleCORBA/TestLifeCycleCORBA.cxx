using namespace std;
//=============================================================================
// File      : TestLifeCycleCORBA.cxx
// Created   : jeu jui 12 13:11:27 CEST 2001
// Author    : Paul RASCLE, EDF - MARC TAJCHMAN, CEA
// Project   : SALOME
// Copyright : EDF 2001
// $Header$
//=============================================================================

#include "utilities.h"
#include <iostream>
#include <unistd.h>
#include <SALOMEconfig.h>
#include CORBA_CLIENT_HEADER(SALOME_Component)
#include CORBA_CLIENT_HEADER(SALOME_TestComponent)
#include "SALOME_NamingService.hxx"
#include "SALOME_LifeCycleCORBA.hxx"
#include <OpUtil.hxx>

int main (int argc, char * argv[])
{

  try
    {
      // Initializing omniORB
      CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
    
      // Obtain a reference to the root POA
      CORBA::Object_var obj = orb->resolve_initial_references("RootPOA") ;
      PortableServer::POA_var poa = PortableServer::POA::_narrow(obj) ;
    
      SALOME_NamingService _NS(orb) ;

      SALOME_LifeCycleCORBA _LCC(&_NS) ;

      // get a local container (with a name based on local hostname),
      // load an engine, and invoque methods on that engine

      string containerName = GetHostname();

      Engines::Component_var mycompo =
	_LCC.FindOrLoad_Component(containerName.c_str(),"SalomeTestComponent");

      ASSERT(!CORBA::is_nil(mycompo));

      Engines::TestComponent_var m1;
      m1 = Engines::TestComponent::_narrow(mycompo);

      ASSERT(!CORBA::is_nil(m1));

      SCRUTE(m1->instanceName());
      MESSAGE("Coucou " << m1->Coucou(1L));

      // get another container (with a fixed name),
      // load an engine, and invoque methods on that engine

      string containerName2 = "FactoryServerPy";

      Engines::Component_var mycompo2 =
	_LCC.FindOrLoad_Component(containerName2.c_str(),"SALOME_TestComponentPy");

      ASSERT(!CORBA::is_nil(mycompo2));

      Engines::TestComponent_var m2;
      m2 = Engines::TestComponent::_narrow(mycompo2);

      ASSERT(!CORBA::is_nil(m2));

      SCRUTE(m2->instanceName());
      MESSAGE("Coucou " << m2->Coucou(1L));
    }
  catch(CORBA::COMM_FAILURE& ex)
    {
      INFOS("Caught system exception COMM_FAILURE -- unable to contact the object.");
    }
  catch(CORBA::SystemException&)
    {
      INFOS("Caught a CORBA::SystemException.");
    }
  catch(CORBA::Exception&)
    {
      INFOS("Caught CORBA::Exception.");
    }
  catch(...)
    {
      INFOS("Caught unknown exception.");
    }

  return 0;
}

