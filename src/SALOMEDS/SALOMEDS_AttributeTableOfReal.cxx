//  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  File   : SALOMEDS_AttributeTableOfReal.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDS_AttributeTableOfReal.hxx"
#include "SALOMEDS.hxx"

using namespace std;

SALOMEDS_AttributeTableOfReal::SALOMEDS_AttributeTableOfReal
                  (SALOMEDSImpl_AttributeTableOfReal* theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeTableOfReal::SALOMEDS_AttributeTableOfReal(SALOMEDS::AttributeTableOfReal_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeTableOfReal::~SALOMEDS_AttributeTableOfReal()
{}


void SALOMEDS_AttributeTableOfReal::SetTitle(const std::string& theTitle)
{
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;     
    dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_local_impl)->SetTitle(theTitle);
  }
  else SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->SetTitle(theTitle.c_str());
}

std::string SALOMEDS_AttributeTableOfReal::GetTitle()
{
  std::string aStr;
  if (_isLocal) {
    SALOMEDS::Locker lock; 
    aStr = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_local_impl)->GetTitle();
  }
  else aStr = SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->GetTitle();
  return aStr;
}

void SALOMEDS_AttributeTableOfReal::SetRowTitle(int theIndex, const std::string& theTitle)
{
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;     
    dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_local_impl)->SetRowTitle(theIndex, theTitle);
  }
  else SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->SetRowTitle(theIndex, theTitle.c_str());
}

void SALOMEDS_AttributeTableOfReal::SetRowTitles(const std::vector<std::string>& theTitles)
{
  CheckLocked();
  int aLength = theTitles.size(), i;
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;     
    dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_local_impl)->SetRowTitles(theTitles);
  }
  else {
    SALOMEDS::StringSeq_var aSeq = new SALOMEDS::StringSeq();
    aSeq->length(aLength);
    for (i = 0; i < aLength; i++) aSeq[i] = (char*)theTitles[i].c_str();
    SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->SetRowTitles(aSeq);
  }
}

std::vector<std::string> SALOMEDS_AttributeTableOfReal::GetRowTitles()
{
  std::vector<std::string> aVector;
  int aLength, i;
  if (_isLocal) {
    SALOMEDS::Locker lock; 
    aVector = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_local_impl)->GetRowTitles();
  }
  else {
    SALOMEDS::StringSeq_var aSeq = SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->GetRowTitles();
    aLength = aSeq->length();
    for (i = 0; i < aLength; i++) aVector.push_back((char*)aSeq[i].in());
  }
  return aVector;
}

void SALOMEDS_AttributeTableOfReal::SetColumnTitle(int theIndex, const std::string& theTitle)
{
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock; 
    dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_local_impl)->SetColumnTitle(theIndex, theTitle);
  }
  else SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->SetColumnTitle(theIndex, theTitle.c_str());
}

void SALOMEDS_AttributeTableOfReal::SetColumnTitles(const std::vector<std::string>& theTitles)
{
  int aLength = theTitles.size(), i;
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock; 
    dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_local_impl)->SetColumnTitles(theTitles);
  }
  else {
    SALOMEDS::StringSeq_var aSeq = new SALOMEDS::StringSeq();
    aSeq->length(aLength);
    for (i = 0; i < aLength; i++) aSeq[i] = (char*)theTitles[i].c_str();
    SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->SetColumnTitles(aSeq);
  }
}

std::vector<std::string> SALOMEDS_AttributeTableOfReal::GetColumnTitles()
{
  std::vector<std::string> aVector;
  int aLength, i;
  if (_isLocal) {
    SALOMEDS::Locker lock; 
    aVector = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_local_impl)->GetColumnTitles();
  }
  else {
    SALOMEDS::StringSeq_var aSeq = SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->GetColumnTitles();
    aLength = aSeq->length();
    for (i = 0; i < aLength; i++) aVector.push_back((char*)aSeq[i].in());
  }
  return aVector;
}

void SALOMEDS_AttributeTableOfReal::SetRowUnit(int theIndex, const std::string& theUnit)
{
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock; 
    dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_local_impl)->SetRowUnit(theIndex, theUnit);
  }
  else SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->SetRowUnit(theIndex, theUnit.c_str());
}

void SALOMEDS_AttributeTableOfReal::SetRowUnits(const std::vector<std::string>& theUnits)
{
  int aLength = theUnits.size(), i;
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock; 
    dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_local_impl)->SetRowUnits(theUnits);
  }
  else {
    SALOMEDS::StringSeq_var aSeq = new SALOMEDS::StringSeq();
    aSeq->length(aLength);
    for (i = 0; i < aLength; i++) aSeq[i] = (char*)theUnits[i].c_str();
    SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->SetRowUnits(aSeq);
  }
}

std::vector<std::string> SALOMEDS_AttributeTableOfReal::GetRowUnits()
{
  std::vector<std::string> aVector;
  int aLength, i;
  if (_isLocal) {
    SALOMEDS::Locker lock; 
    aVector = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_local_impl)->GetRowUnits();
  }
  else {
    SALOMEDS::StringSeq_var aSeq = SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->GetRowUnits();
    aLength = aSeq->length();
    for (i = 0; i < aLength; i++) aVector.push_back(string(aSeq[i].in()));
  }
  return aVector;
}

int SALOMEDS_AttributeTableOfReal::GetNbRows()
{
  int aNb;
  if (_isLocal) {
    SALOMEDS::Locker lock; 
    aNb = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_local_impl)->GetNbRows();
  }
  else aNb = SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->GetNbRows();
  return aNb;
}

int SALOMEDS_AttributeTableOfReal::GetNbColumns()
{  
  int aNb;
  if (_isLocal) {
    SALOMEDS::Locker lock; 
    aNb = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_local_impl)->GetNbColumns();
  }
  else aNb = SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->GetNbColumns();
  return aNb;
}

void SALOMEDS_AttributeTableOfReal::AddRow(const std::vector<double>& theData)
{
  int aLength = theData.size(), i;
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock; 
    SALOMEDSImpl_AttributeTableOfReal* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_local_impl);
    try {
      aTable->SetRowData(aTable->GetNbRows() + 1, theData);
    }   
    catch(...) {
      throw SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength();
    }
  }
  else {
    SALOMEDS::DoubleSeq_var aSeq = new SALOMEDS::DoubleSeq();
    aSeq->length(aLength);
    for (i = 0; i < aLength; i++) aSeq[i] = theData[i];
    SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->AddRow(aSeq);
  }
}

void SALOMEDS_AttributeTableOfReal::SetRow(int theRow, const std::vector<double>& theData)
{
  int aLength = theData.size(), i;
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock; 
    SALOMEDSImpl_AttributeTableOfReal* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_local_impl);
    try {
      aTable->SetRowData(theRow, theData);
    }   
    catch(...) {
      throw SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength();
    }
  }
  else {
    SALOMEDS::DoubleSeq_var aSeq = new SALOMEDS::DoubleSeq();
    aSeq->length(aLength);
    for (i = 0; i < aLength; i++) aSeq[i] = theData[i];
    SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->SetRow(theRow, aSeq);
  }
}

std::vector<double> SALOMEDS_AttributeTableOfReal::GetRow(int theRow)
{
  std::vector<double> aVector;
  int aLength, i;
  if (_isLocal) {
    SALOMEDS::Locker lock; 
    aVector = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_local_impl)->GetRowData(theRow);
  }
  else {
    SALOMEDS::DoubleSeq_var aRow = SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->GetRow(theRow);
    aLength = aRow->length();
    for (i = 0; i < aLength; i++) aVector.push_back(aRow[i]);
  }

  return aVector;
}

void SALOMEDS_AttributeTableOfReal::AddColumn(const std::vector<double>& theData)
{
  int aLength = theData.size(), i;
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock; 
    SALOMEDSImpl_AttributeTableOfReal* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_local_impl);
    try {
      aTable->SetColumnData(aTable->GetNbColumns() + 1, theData);
    }   
    catch(...) {
      throw SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength();
    }
  }
  else {
    SALOMEDS::DoubleSeq_var aColumn = new SALOMEDS::DoubleSeq();
    aColumn->length(aLength);
    for (i = 0; i < aLength; i++) aColumn[i] = theData[i];
    SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->AddColumn(aColumn);
  }
}

void SALOMEDS_AttributeTableOfReal::SetColumn(int theColumn, const std::vector<double>& theData)
{
  int aLength = theData.size(), i;
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock; 
    SALOMEDSImpl_AttributeTableOfReal* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_local_impl);
    try {
      aTable->SetColumnData(theColumn, theData);
    }   
    catch(...) {
      throw SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength();
    }
  }
  else {
    SALOMEDS::DoubleSeq_var aColumn = new SALOMEDS::DoubleSeq();
    aColumn->length(aLength);
    for (i = 0; i < aLength; i++) aColumn[i] = theData[i];
    SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->SetColumn(theColumn, aColumn);
  }
}

std::vector<double> SALOMEDS_AttributeTableOfReal::GetColumn(int theColumn)
{
  std::vector<double> aVector;
  int aLength, i;
  if (_isLocal) {
    SALOMEDS::Locker lock; 
    aVector = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_local_impl)->GetColumnData(theColumn);
  }
  else {
    SALOMEDS::DoubleSeq_var aColumn = SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->GetColumn(theColumn);
    aLength = aColumn->length();
    for (i = 0; i < aLength; i++) aVector.push_back(aColumn[i]);
  }
  return aVector;
}

void SALOMEDS_AttributeTableOfReal::PutValue(double theValue, int theRow, int theColumn)
{
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock; 
    try {
      dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_local_impl)->PutValue(theValue, theRow, theColumn);
    }   
    catch(...) {
      throw SALOMEDS::AttributeTableOfReal::IncorrectIndex();
    }
  }
  else {
    SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->PutValue(theValue, theRow, theColumn);
  }
}

bool SALOMEDS_AttributeTableOfReal::HasValue(int theRow, int theColumn)
{
  bool ret;
  if (_isLocal) {
    SALOMEDS::Locker lock; 
    ret = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_local_impl)->HasValue(theRow, theColumn);
  }
  else ret = SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->HasValue(theRow, theColumn);
  return ret;
}

double SALOMEDS_AttributeTableOfReal::GetValue(int theRow, int theColumn)
{
  double aValue;
  if (_isLocal) {
    SALOMEDS::Locker lock; 
    try {
      aValue = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_local_impl)->GetValue(theRow, theColumn);
    }   
    catch(...) {
      throw SALOMEDS::AttributeTableOfReal::IncorrectIndex();
    }
  }
  else {
    aValue = SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->GetValue(theRow, theColumn);
  }
  return aValue;
}

std::vector<int> SALOMEDS_AttributeTableOfReal::GetRowSetIndices(int theRow)
{
  std::vector<int> aVector;
  int aLength, i;
  if (_isLocal) {
    SALOMEDS::Locker lock; 
    aVector = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_local_impl)->GetSetRowIndices(theRow);
  }
  else {
    SALOMEDS::LongSeq_var aSet = SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->GetRowSetIndices(theRow);
    aLength = aSet->length();
    for (i = 0; i < aLength; i++) aVector.push_back(aSet[i]);
  }
  return aVector;
}

void SALOMEDS_AttributeTableOfReal::SetNbColumns(int theNbColumns)
{
  if (_isLocal) {
    SALOMEDS::Locker lock; 
    dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_local_impl)->SetNbColumns(theNbColumns);
  }
  else SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->SetNbColumns(theNbColumns);
}
