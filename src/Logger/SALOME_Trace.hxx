//=============================================================================
// File      : SALOME_Trace.cxx
// Created   : nov 18 10:28:17 2002
// Author    : Vasily Rusyaev
// Project   : SALOME/PRO
//=============================================================================

// SALOME_Trace.hxx: interface for the SALOME_Trace class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGGER_H__96F2A3AB_F6F8_11D6_BFA6_000476A0958C__INCLUDED_)
#define AFX_LOGGER_H__96F2A3AB_F6F8_11D6_BFA6_000476A0958C__INCLUDED_

//these declarations for files don't using OCC includes (for example HDF)
# ifdef WNT

#  ifndef Standard_EXPORT
#   define Standard_EXPORT __declspec( dllexport )
// For global variables :
#   define Standard_EXPORTEXTERN __declspec( dllexport ) extern
#   define Standard_EXPORTEXTERNC extern "C" __declspec( dllexport )
#  endif  /* Standard_EXPORT */

#  ifndef Standard_IMPORT
#   define Standard_IMPORT __declspec( dllimport ) extern
#   define Standard_IMPORTC extern "C" __declspec( dllimport )
#  endif  /* Standard_IMPORT */

# else  /* WNT */

#  ifndef Standard_EXPORT
#   define Standard_EXPORT
// For global variables :
#   define Standard_EXPORTEXTERN extern
#   define Standard_EXPORTEXTERNC extern "C"
#  endif  /* Standard_EXPORT */

#  ifndef Standard_IMPORT
#   define Standard_IMPORT extern
#   define Standard_IMPORTC extern "C"
#  endif  /* Standard_IMPORT */

# endif  /* WNT */

#include <strstream.h>
#include "Logger.hh"

class SALOME_Trace : public ostrstream  
{
public:
	virtual ~SALOME_Trace();
        static Standard_EXPORT SALOME_Trace& Instance();
	Standard_EXPORT void putMessage(ostream& msg);
protected:
	//disable creation of instances. It's necessary to use static SALOME_Logger& Instance()
	SALOME_Trace();
	SALOME_Logger::Logger_var m_pInterfaceLogger;// object reference on Logger server.
};

#define GLogger SALOME_Trace::Instance()

#endif // !defined(AFX_LOGGER_H__96F2A3AB_F6F8_11D6_BFA6_000476A0958C__INCLUDED_)
