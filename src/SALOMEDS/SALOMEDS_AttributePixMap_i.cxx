//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
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
//  File   : SALOMEDS_AttributePixMap_i.cxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header$

using namespace std;
#include "SALOMEDS_AttributePixMap_i.hxx"
#include "SALOMEDS_SObject_i.hxx"
#include <TCollection_AsciiString.hxx>

CORBA::Boolean SALOMEDS_AttributePixMap_i::HasPixMap() {
  TCollection_ExtendedString S = Handle(SALOMEDS_PixMapAttribute)::DownCast(_myAttr)->Get();
  if (strcmp(TCollection_AsciiString(S).ToCString(), "None") == 0) return Standard_False;
  return Standard_True;
}

char* SALOMEDS_AttributePixMap_i::GetPixMap() {
 CORBA::String_var S = CORBA::string_dup(TCollection_AsciiString(Handle(SALOMEDS_PixMapAttribute)::DownCast(_myAttr)->Get()).ToCString());
 return S._retn();
}

void SALOMEDS_AttributePixMap_i::SetPixMap(const char* value) {
  CheckLocked();
  CORBA::String_var Str = CORBA::string_dup(value);
  Handle(TDataStd_Comment)::DownCast(_myAttr)->Set(TCollection_ExtendedString(Str));
}
