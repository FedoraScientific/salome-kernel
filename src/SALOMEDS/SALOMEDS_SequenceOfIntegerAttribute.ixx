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
//  File   : SALOMEDS_SequenceOfIntegerAttribute.ixx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header$

#include "SALOMEDS_SequenceOfIntegerAttribute.jxx"

#ifndef _Standard_TypeMismatch_HeaderFile
#include <Standard_TypeMismatch.hxx>
#endif

SALOMEDS_SequenceOfIntegerAttribute::~SALOMEDS_SequenceOfIntegerAttribute() {}
 


Standard_EXPORT Handle_Standard_Type& SALOMEDS_SequenceOfIntegerAttribute_Type_()
{

    static Handle_Standard_Type aType1 = STANDARD_TYPE(TDF_Attribute);
  if ( aType1.IsNull()) aType1 = STANDARD_TYPE(TDF_Attribute);
  static Handle_Standard_Type aType2 = STANDARD_TYPE(MMgt_TShared);
  if ( aType2.IsNull()) aType2 = STANDARD_TYPE(MMgt_TShared);
  static Handle_Standard_Type aType3 = STANDARD_TYPE(Standard_Transient);
  if ( aType3.IsNull()) aType3 = STANDARD_TYPE(Standard_Transient);
 

  static Handle_Standard_Transient _Ancestors[]= {aType1,aType2,aType3,NULL};
  static Handle_Standard_Type _aType = new Standard_Type("SALOMEDS_SequenceOfIntegerAttribute",
			                                 sizeof(SALOMEDS_SequenceOfIntegerAttribute),
			                                 1,
			                                 (Standard_Address)_Ancestors,
			                                 (Standard_Address)NULL);

  return _aType;
}


// DownCast method
//   allow safe downcasting
//
const Handle(SALOMEDS_SequenceOfIntegerAttribute) Handle(SALOMEDS_SequenceOfIntegerAttribute)::DownCast(const Handle(Standard_Transient)& AnObject) 
{
  Handle(SALOMEDS_SequenceOfIntegerAttribute) _anOtherObject;

  if (!AnObject.IsNull()) {
     if (AnObject->IsKind(STANDARD_TYPE(SALOMEDS_SequenceOfIntegerAttribute))) {
       _anOtherObject = Handle(SALOMEDS_SequenceOfIntegerAttribute)((Handle(SALOMEDS_SequenceOfIntegerAttribute)&)AnObject);
     }
  }

  return _anOtherObject ;
}
const Handle(Standard_Type)& SALOMEDS_SequenceOfIntegerAttribute::DynamicType() const 
{ 
  return STANDARD_TYPE(SALOMEDS_SequenceOfIntegerAttribute) ; 
}
Standard_Boolean SALOMEDS_SequenceOfIntegerAttribute::IsKind(const Handle(Standard_Type)& AType) const 
{ 
  return (STANDARD_TYPE(SALOMEDS_SequenceOfIntegerAttribute) == AType || TDF_Attribute::IsKind(AType)); 
}
Handle_SALOMEDS_SequenceOfIntegerAttribute::~Handle_SALOMEDS_SequenceOfIntegerAttribute() {}

