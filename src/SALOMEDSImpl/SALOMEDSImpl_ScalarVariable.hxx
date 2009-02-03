// Copyright (C) 2008  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public  
// License along with this library; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  File   : SALOMEDSImpl_ScalarVariable.hxx
//  Author : Roman NIKOLAEV, Open CASCADE S.A.S.
//  Module : SALOME

#ifndef _SALOMEDSImpl_ScalarVariable_HeaderFile
#define _SALOMEDSImpl_ScalarVariable_HeaderFile

#include "SALOMEDSImpl_Defines.hxx"
#include "SALOMEDSImpl_GenericVariable.hxx"

class SALOMEDSIMPL_EXPORT SALOMEDSImpl_ScalarVariable : 
  public SALOMEDSImpl_GenericVariable 
{
public:
  SALOMEDSImpl_ScalarVariable(SALOMEDSImpl_GenericVariable::VariableTypes theType,
                              const std::string& theName);
  ~SALOMEDSImpl_ScalarVariable();
  
  bool setValue(const double theValue);
  double getValue() const;

  virtual std::string Save() const;
  virtual std::string SaveToScript() const;
  virtual std::string SaveType() const;

  virtual void Load(const std::string& theStrValue);

private:
  double myValue;
};


#endif //_SALOMEDSImpl_ScalarVariable_HeaderFile
