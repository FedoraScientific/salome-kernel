//  File   : SALOMEDS_AttributeIOR_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME


#include "SALOMEDS_AttributeIOR_i.hxx"
#include "SALOMEDS.hxx"
#include <TCollection_ExtendedString.hxx>

using namespace std;

char* SALOMEDS_AttributeIOR_i::Value()
{
 SALOMEDS::Locker lock;
  CORBA::String_var c_s = 
    CORBA::string_dup(TCollection_AsciiString(Handle(SALOMEDSImpl_AttributeIOR)::DownCast(_impl)->Value()).ToCString());
  return c_s._retn();
}

void SALOMEDS_AttributeIOR_i::SetValue(const char* value) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  CORBA::String_var Str = CORBA::string_dup(value);
  Handle(SALOMEDSImpl_AttributeIOR)::DownCast(_impl)->SetValue(TCollection_ExtendedString(Str));
}
