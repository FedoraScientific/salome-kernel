//  SALOME Utils : general SALOME's definitions and tools
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
//  File   : Utils_ORB_INIT.cxx
//  Author : Antoine YESSAYAN, EDF
//  Module : SALOME
//  $Header$

using namespace std;
# include "Utils_ORB_INIT.hxx" 
# include "utilities.h" 

ORB_INIT::ORB_INIT( void ): _orb( CORBA::ORB::_nil() )
{
	;
}


ORB_INIT::~ORB_INIT()
{
	if ( ! CORBA::is_nil( _orb ) )
	{
		MESSAGE("appel _orb->destroy()") ;
		_orb->destroy() ;
		MESSAGE("retour _orb->destroy()") ;
	}
}


CORBA::ORB_var &ORB_INIT::operator() ( int argc , char **argv ) throw( CommException )
{
	if ( CORBA::is_nil( _orb ) )
	{
		try
		{
		  //		  const char* options[][2] = { { "giopMaxMsgSize", "104857600" }, { 0, 0 } };
		  //		  _orb = CORBA::ORB_init( argc , argv , "omniORB4", options) ;
		  _orb = CORBA::ORB_init( argc , argv ) ;
		  //set GIOP message size equal to 50Mb for transferring brep shapes as 
		  //sequence of bytes using C++ streams
		  omniORB::MaxMessageSize(100*1024*1024);
		}
		catch( const CORBA::Exception &ex )
		{
			throw CommException( "Unable to create an ORB connexion" ) ;
		}
	}
	return _orb ;
}
