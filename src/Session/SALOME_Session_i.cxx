using namespace std;
//=============================================================================
// File      : SALOME_Session_i.cxx
// Created   : mar jun 19 14:02:45 CEST 2001
// Author    : Paul RASCLE, EDF
// Project   : SALOME
// Copyright : EDF 2001
// $Header$
//=============================================================================

#include "utilities.h"

#include "SALOME_Session_i.hxx"
#include "SALOME_NamingService.hxx"
#include "SALOME_Session_QThread.hxx"

#include "QAD_Application.h"
#include "QAD_Desktop.h"
#include <qapplication.h>

// Open CASCADE Includes
#include <OSD_SharedLibrary.hxx>
#include <OSD_LoadMode.hxx>
#include <OSD_Function.hxx>

//=============================================================================
/*! SALOME_Session_i
 *  constructor
 */ 
//=============================================================================

SALOME_Session_i::SALOME_Session_i(int argc, char ** argv, CORBA::ORB_ptr orb, PortableServer::POA_ptr poa)
{
  _argc = argc ;
  _argv = argv ;
  _IAPPThread = new SALOME_Session_QThread(_argc, _argv) ;
  _isGUI = FALSE ;
  _runningStudies= 0 ;
  _orb = CORBA::ORB::_duplicate(orb) ;
  _poa = PortableServer::POA::_duplicate(poa) ;
  MESSAGE("constructor end");
}
  
//***//VISU::VISU_Gen_ptr SALOME_Session_i::GetVisuGen(){
//***//  typedef VISU::VISU_Gen_ptr VisuGen(CORBA::ORB_var,PortableServer::POA_var,QMutex*);
//***//  MESSAGE("SALOME_Session_i::GetVisuGen");
//***//  OSD_SharedLibrary  visuSharedLibrary("libVisuEngine.so");
//***//  if(visuSharedLibrary.DlOpen(OSD_RTLD_LAZY))
//***//    if(OSD_Function osdFun = visuSharedLibrary.DlSymb("GetVisuGen"))
//***//      return ((VisuGen (*)) osdFun)(_orb,_poa,&_GUIMutex);
//***//  return VISU::VISU_Gen::_nil();
//***//} 

//=============================================================================
/*! ~SALOME_Session_i
 *  destructor
 */ 
//=============================================================================

SALOME_Session_i::~SALOME_Session_i()
{
  MESSAGE("destructor end"); 
}

//=============================================================================
/*! NSregister
 *  tries to find the Corba Naming Service and to register the session,
 *  gives naming service interface to _IAPPThread
 */ 
//=============================================================================

void SALOME_Session_i::NSregister()
{
  SALOME::Session_ptr pSession = SALOME::Session::_narrow(_this());
  try
    {
      _NS = new SALOME_NamingService(_orb);
      _NS->Register(pSession, "/Kernel/Session");
      _IAPPThread->setNamingService(_NS);
    }
  catch (ServiceUnreachable&)
    {
      INFOS("Caught exception: Naming Service Unreachable");
      exit(1) ;
    }
  catch (...)
    {
      INFOS("Caught unknown exception from Naming Service");
    }
  MESSAGE("NSregister end"); 
}

//=============================================================================
/*! GetInterface
 *  Launches the GUI if there is none.
 *  The Corba method is oneway (corba client does'nt wait for GUI completion)
 */ 
//=============================================================================

void SALOME_Session_i::GetInterface()
{
  _GUIMutex.lock() ;       // get access to boolean _isGUI
  _isGUI = _IAPPThread->running();
  if(!_isGUI){
    _isGUI = TRUE ; 
    _IAPPThread->start() ;
  }
  _GUIMutex.unlock() ; // release access to boolean _isGUI 
}

//=============================================================================
/*! StopSession
 *  Kills the session if there are no active studies nore GUI
 */ 
//=============================================================================

void SALOME_Session_i::StopSession()
{
  qApp->lock();
  QAD_Application::getDesktop()->closeDesktop( true );
  qApp->unlock();
/*
  _GUIMutex.lock();         // get access to boolean _isGUI
  if ((! _isGUI) && (! _runningStudies))
    {
      MESSAGE("Ask for Session Kill, OK");
      exit(0);
    }
  else
    {
      _GUIMutex.unlock() ;  // release access to boolean _isGUI
      MESSAGE("Ask for Session Kill, NOK");
      if (_isGUI) throw SALOME::Session::GUIActive();   
      if (_runningStudies) throw SALOME::Session::RunningStudies();
    }
*/
}
 
//=============================================================================
/*! StatSession
 *  Send a SALOME::StatSession structure (see idl) to the client
 *  (number of running studies and presence of GUI)
 */ 
//=============================================================================

SALOME::StatSession SALOME_Session_i::GetStatSession()
{
  // update Session state
  _GUIMutex.lock();    
  _isGUI = _IAPPThread->running();
  _runningStudies = 0;
  if (_isGUI) {
    qApp->lock();
    if ( QAD_Application::getDesktop() && QAD_Application::getDesktop()->getActiveApp() )
      _runningStudies = QAD_Application::getDesktop()->getActiveApp()->getStudies().count();
    qApp->unlock();
  }
  _GUIMutex.unlock();
  // getting stat info
  SALOME::StatSession_var myStats = new SALOME::StatSession ;
  if (_runningStudies)
    myStats->state = SALOME::running ;
  else
    myStats->state = SALOME::asleep ;
  myStats->runningStudies = _runningStudies ;
  myStats->activeGUI = _isGUI ;
  return myStats._retn() ;
}

