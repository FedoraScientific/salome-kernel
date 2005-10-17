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
//  File   : Utils_CommException.hxx
//  Author : Antoine YESSAYAN, EDF
//  Module : SALOME
//  $Header$

# if  !defined ( __Utils_CommException_H__ )
# define __Utils_CommException_H__ )

# include "Utils_SALOME_Exception.hxx"

class UTILS_EXPORT CommException : public SALOME_Exception
{
public :
	CommException( void );
	CommException( const char *texte );
	CommException( const CommException &ex );
	~CommException() throw ();
} ;

# endif	/* # if ( !defined __Utils_CommException_H__ ) */
