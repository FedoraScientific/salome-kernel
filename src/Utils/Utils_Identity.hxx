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
//  File   : Utils_Identity.hxx
//  Author : Pascale NOYRET, EDF
//  Module : SALOME
//  $Header$

# if !defined(  __IDENTITE_H__ )
# define __IDENTITE_H__

extern "C"
{
# include <stdlib.h>
# include <time.h>
#ifndef WNT
# include <unistd.h>
# include <sys/utsname.h>
#else
# include <windows.h>
#endif
}

#if defined UTILS_EXPORTS
#if defined WIN32
#define UTILS_EXPORT __declspec( dllexport )
#else
#define UTILS_EXPORT
#endif
#else
#if defined WNT
#define UTILS_EXPORT __declspec( dllimport )
#else
#define UTILS_EXPORT
#endif
#endif
class UTILS_EXPORT Identity
{

protected :
	const char* const	_name ;
	const char* const	_adip; // Internet address

#ifndef WNT
        const struct utsname    _hostid;        
	const pid_t		_pid ;
	const uid_t		_uid ;
#else
	const char* const       _hostid;
	const DWORD		_pid ;
	const PSID		_uid ;
#endif	
        const char* const	_pwname ;
	const char* const	_dir ;
	const time_t		_start;
	const char* const	_cstart ;

private :
	Identity( void );
	Identity( const Identity &monid );

public :
	Identity(const char *name);
	~Identity();
	friend std::ostream & operator<< ( std::ostream& os , const Identity& monid );

#ifndef WNT
	const pid_t&	        pid(void) const;
        const struct utsname&	hostid(void) const;
	const uid_t&		uid(void) const;
#else
	const DWORD&	        pid(void) const;
        const char* const       hostid(void) const;
	const PSID&	    	uid(void) const;
#endif

	const char* const	name( void ) const;
	const char* const	adip(void) const;
	const char* const	pwname(void) const;
	const time_t&		start(void) const;
	const char* const 	rep (void) const;

	const char*		host_char(void ) const;
	const char*		start_char(void) const;

} ;
# endif		/* # if !defined(  __IDENTITE_H__ ) */
