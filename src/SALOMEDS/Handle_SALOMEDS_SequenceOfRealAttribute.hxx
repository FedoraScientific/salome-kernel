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
//  File   : Handle_SALOMEDS_SequenceOfRealAttribute.hxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header$

#ifndef _Handle_SALOMEDS_SequenceOfRealAttribute_HeaderFile
#define _Handle_SALOMEDS_SequenceOfRealAttribute_HeaderFile

#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif
#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif

#ifndef _Handle_TDF_Attribute_HeaderFile
#include <Handle_TDF_Attribute.hxx>
#endif

class Standard_Transient;
class Handle_Standard_Type;
class Handle(TDF_Attribute);
class SALOMEDS_SequenceOfRealAttribute;
Standard_EXPORT Handle_Standard_Type& STANDARD_TYPE(SALOMEDS_SequenceOfRealAttribute);

class Handle(SALOMEDS_SequenceOfRealAttribute) : public Handle(TDF_Attribute) {
  public:
    void* operator new(size_t,void* anAddress) 
      {
        return anAddress;
      }
    void* operator new(size_t size) 
      { 
        return Standard::Allocate(size); 
      }
    void  operator delete(void *anAddress) 
      { 
        if (anAddress) Standard::Free((Standard_Address&)anAddress); 
      }
    Handle(SALOMEDS_SequenceOfRealAttribute)():Handle(TDF_Attribute)() {} 
    Handle(SALOMEDS_SequenceOfRealAttribute)(const Handle(SALOMEDS_SequenceOfRealAttribute)& aHandle) : Handle(TDF_Attribute)(aHandle) 
     {
     }

    Handle(SALOMEDS_SequenceOfRealAttribute)(const SALOMEDS_SequenceOfRealAttribute* anItem) : Handle(TDF_Attribute)((TDF_Attribute *)anItem) 
     {
     }

    Handle(SALOMEDS_SequenceOfRealAttribute)& operator=(const Handle(SALOMEDS_SequenceOfRealAttribute)& aHandle)
     {
      Assign(aHandle.Access());
      return *this;
     }

    Handle(SALOMEDS_SequenceOfRealAttribute)& operator=(const SALOMEDS_SequenceOfRealAttribute* anItem)
     {
      Assign((Standard_Transient *)anItem);
      return *this;
     }

    SALOMEDS_SequenceOfRealAttribute* operator->() 
     {
      return (SALOMEDS_SequenceOfRealAttribute *)ControlAccess();
     }

    SALOMEDS_SequenceOfRealAttribute* operator->() const 
     {
      return (SALOMEDS_SequenceOfRealAttribute *)ControlAccess();
     }

   Standard_EXPORT ~Handle(SALOMEDS_SequenceOfRealAttribute)();
 
   Standard_EXPORT static const Handle(SALOMEDS_SequenceOfRealAttribute) DownCast(const Handle(Standard_Transient)& AnObject);
};
#endif
