//  SALOME Registry : Registry server implementation
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
//  File   : RegistryConnexion.hxx
//  Author : Pascale NOYRET - Antoine YESSAYAN, EDF
//  Module : SALOME
//  $Header$

/*
	RegistryConnexion should be used by an engine to add or to remove a component.

*/
# if !defined( __RegistryConnexion_HXX__ )
# define __RegistryConnexion_HXX__

#include <SALOMEconfig.h>
#include CORBA_CLIENT_HEADER(SALOME_Registry)
#include <string>

#if defined REGISTRY_EXPORTS
#if defined WIN32
#define REGISTRY_EXPORT __declspec( dllexport )
#else
#define REGISTRY_EXPORT
#endif
#else
#if defined WNT
#define REGISTRY_EXPORT __declspec( dllimport )
#else
#define REGISTRY_EXPORT
#endif
#endif

class REGISTRY_EXPORT RegistryConnexion
{
protected :
	const char*			_Ior		; // engine ior
	Registry::Components_var	_VarComponents	; // RegistryService reference
	std::string			_SessionName	;
	std::string			_Name		; // code name
	int   				_Id   		; // code identity returned by RegistryService
	void add( const char *aName );
	void remove( void );
	RegistryConnexion( void );

public :
	RegistryConnexion( int argc , char **argv , const char *ior , const char *ptrSessionName, const char *componentName );
	~RegistryConnexion();
} ;

# endif		/* # if !defined( __RegistryConnexion_H__ ) */
