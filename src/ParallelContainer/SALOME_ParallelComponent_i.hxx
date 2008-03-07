//  SALOME_ParallelComponent : implementation of container and engine for Parallel Kernel
//
//  Copyright (C) 2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  File   : SALOME_ParallelComponent_i.hxx
//  Author : Andr� RIBES, EDF
//  Author : Paul RASCLE, EDF - MARC TAJCHMAN, CEA

#ifndef _SALOME_PARALLEL_COMPONENT_I_HXX_
#define _SALOME_PARALLEL_COMPONENT_I_HXX_

#include <iostream>
#include <signal.h>
#include <stdlib.h>
#ifndef WNT
#include <unistd.h>
#endif
#include <sys/types.h>
#include <string>
#include <map>
#include <SALOMEconfig.h>

#include "SALOME_ComponentPaCO_Engines_Parallel_Component_server.h"

#include "NOTIFICATION.hxx"
#include "RegistryConnexion.hxx"
#include "Parallel_Salome_file_i.hxx"

class Engines_Parallel_Container_i;

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

class CONTAINER_EXPORT Engines_Parallel_Component_i: 
  public virtual Engines::Parallel_Component_serv,
  public virtual PortableServer::RefCountServantBase
{
public:
  Engines_Parallel_Component_i(CORBA::ORB_ptr orb, char * ior, int rank);
  Engines_Parallel_Component_i(CORBA::ORB_ptr orb, char * ior, int rank,
		               PortableServer::POA_ptr poa,
			       PortableServer::ObjectId * contId, 
			       const char *instanceName, 
			       const char *interfaceName,
			       bool notif = false);

  virtual ~Engines_Parallel_Component_i();

  // --- CORBA methods

  char* instanceName();
  char* interfaceName();

  void ping();
  void destroy();

  CORBA::Long getStudyId();
  Engines::Container_ptr GetContainerRef();

  void setProperties(const Engines::FieldsDict& dico);
  Engines::FieldsDict* getProperties();

  void Names( const char * graphName , const char * nodeName ) ;
  bool Kill_impl();
  bool Stop_impl();
  bool Suspend_impl();
  bool Resume_impl();
  CORBA::Long CpuUsed_impl() ;

 virtual Engines::TMPFile* DumpPython(CORBA::Object_ptr theStudy,
				      CORBA::Boolean isPublished,
				      CORBA::Boolean& isValidScript);

 // CORBA operations for Salome_file
 virtual Engines::Salome_file_ptr getInputFileToService(const char* service_name, 
							const char* Salome_file_name);
 virtual Engines::Salome_file_ptr getOutputFileToService(const char* service_name, 
							 const char* Salome_file_name);

 virtual void checkInputFilesToService(const char* service_name);
 virtual Engines::Salome_file_ptr setInputFileToService(const char* service_name, 
							const char* Salome_file_name);

 virtual void checkOutputFilesToService(const char* service_name);
 virtual Engines::Salome_file_ptr setOutputFileToService(const char* service_name, 
							 const char* Salome_file_name);

 void send_parallel_proxy_object(CORBA::Object_ptr proxy_ref);

  // --- local C++ methods

  PortableServer::ObjectId * getId(); 
  Engines_Parallel_Container_i *GetContainerPtr();

  bool setStudyId(CORBA::Long studyId);
  static bool isMultiStudy();
  static bool isMultiInstance();
  static std::string GetDynLibraryName(const char *componentName);

  void beginService(const char *serviceName);
  void endService(const char *serviceName);
  void sendMessage(const char *event_type, const char *message);
  char * graphName() ;
  char * nodeName() ;
  bool Killer( pthread_t ThreadId , int signum );
  void SetCurCpu() ;
  long CpuUsed() ;

  void wait_parallel_object_proxy();
  char * get_parallel_proxy_object();

  virtual void configureSalome_file(std::string service_name,
				    std::string file_port_name,
				    Engines::Parallel_Salome_file_proxy_impl * file);

protected:
  int _studyId; // -1: not initialised; 0: multiStudy; >0: study
  static bool _isMultiStudy;
  static bool _isMultiInstance;

  std::string _instanceName ;
  std::string _interfaceName ;

  CORBA::ORB_ptr _orb;
  PortableServer::POA_ptr _poa;
  PortableServer::ObjectId * _id;
  PortableServer::ObjectId * _contId;
  Engines_Parallel_Component_i * _thisObj ;
  RegistryConnexion *_myConnexionToRegistry;
  NOTIFICATION_Supplier* _notifSupplier;
  std::map<std::string,CORBA::Any>_fieldsDict;

  // Map Salome_file_name to Parallel_Salome_file*
  typedef std::map<std::string, Parallel_Salome_file_i*> _t_Salome_file_map;
  typedef std::map<std::string, Engines::Parallel_Salome_file_proxy_impl*> _t_Proxy_Salome_file_map;
  typedef std::map<std::string, std::string> _t_IOR_Proxy_Salome_file_map;

  // Map Service_name to  _Salome_file_map
  typedef std::map<std::string, Engines_Parallel_Component_i::_t_Salome_file_map*> _t_Service_file_map;
  typedef std::map<std::string, Engines_Parallel_Component_i::_t_Proxy_Salome_file_map*> _t_Proxy_Service_file_map;
  typedef std::map<std::string, Engines_Parallel_Component_i::_t_IOR_Proxy_Salome_file_map*> _t_IOR_Proxy_Service_file_map;
  
  _t_Service_file_map _Input_Service_file_map;
  _t_Service_file_map _Output_Service_file_map;
  _t_Service_file_map::iterator _Service_file_map_it;
  _t_Salome_file_map::iterator _Salome_file_map_it;

  _t_Proxy_Service_file_map _Proxy_Input_Service_file_map;
  _t_Proxy_Service_file_map _Proxy_Output_Service_file_map;
  _t_Proxy_Service_file_map::iterator _Proxy_Service_file_map_it;
  _t_Proxy_Salome_file_map::iterator _Proxy_Salome_file_map_it;

  _t_IOR_Proxy_Service_file_map _IOR_Proxy_Input_Service_file_map;
  _t_IOR_Proxy_Service_file_map _IOR_Proxy_Output_Service_file_map;
  _t_IOR_Proxy_Service_file_map::iterator _IOR_Proxy_Service_file_map_it;
  _t_IOR_Proxy_Salome_file_map::iterator _IOR_Proxy_Salome_file_map_it;

  std::string _serviceName ;
  std::string _graphName ;
  std::string _nodeName ;

  pthread_mutex_t * deploy_mutex;
  char * _proxy;

private:
#ifndef WNT
  pthread_t _ThreadId ;
#else
  pthread_t* _ThreadId ;
#endif
  long      _StartUsed ;
  long      _ThreadCpuUsed ;
  bool      _Executed ;
};

#endif
