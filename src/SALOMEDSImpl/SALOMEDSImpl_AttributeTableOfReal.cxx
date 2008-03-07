// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  File   : SALOMEDSImpl_AttributeTableOfReal.cxx
//  Author : Michael Ponikarov
//  Module : SALOME

#include "SALOMEDSImpl_AttributeTableOfReal.hxx"
#include <stdio.h>
#include <strstream>

using namespace std;

#define SEPARATOR '\1'

typedef map<int, double>::const_iterator MI;

static std::string getUnit(std::string theString)
{
  std::string aString(theString);
  int aPos = aString.find(SEPARATOR);
  if(aPos <= 0 || aPos == aString.size() ) return std::string();
  return aString.substr(aPos+1, aString.size());
}

static std::string getTitle(std::string theString)
{
  std::string aString(theString);
  int aPos = aString.find(SEPARATOR);
  if(aPos < 0) return aString;
  if(aPos == 0) return std::string();
  return aString.substr(0, aPos);
}

const std::string& SALOMEDSImpl_AttributeTableOfReal::GetID() 
{
  static std::string SALOMEDSImpl_AttributeTableOfRealID ("128371A1-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDSImpl_AttributeTableOfRealID;
}

SALOMEDSImpl_AttributeTableOfReal* SALOMEDSImpl_AttributeTableOfReal::Set(const DF_Label& label) 
{
  SALOMEDSImpl_AttributeTableOfReal* A = NULL;
  if (!(A=(SALOMEDSImpl_AttributeTableOfReal*)label.FindAttribute(SALOMEDSImpl_AttributeTableOfReal::GetID()))) {
    A = new SALOMEDSImpl_AttributeTableOfReal();
    label.AddAttribute(A);
  }
  return A;
}

SALOMEDSImpl_AttributeTableOfReal::SALOMEDSImpl_AttributeTableOfReal() 
:SALOMEDSImpl_GenericAttribute("AttributeTableOfReal")
{
  myNbRows = 0;
  myNbColumns = 0;
}

void SALOMEDSImpl_AttributeTableOfReal::SetNbColumns(const int theNbColumns)
{
  CheckLocked();  
  Backup();
  
  map<int, double> aMap;
  aMap = myTable;
  myTable.clear();

  for(MI p = aMap.begin(); p != aMap.end(); p++) {
    int aRow = (int)(p->first/myNbColumns) + 1;
    int aCol = (int)(p->first - myNbColumns*(aRow-1));
    if(aCol == 0) { aCol = myNbColumns; aRow--; }
    if(aCol > theNbColumns) continue;
    int aKey = (aRow-1)*theNbColumns+aCol;
    myTable[aKey] = p->second;
  }

  myNbColumns = theNbColumns;

  while (myCols.size() < myNbColumns) { // append empty columns titles
    myCols.push_back(string(""));
  }
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

void SALOMEDSImpl_AttributeTableOfReal::SetTitle(const std::string& theTitle) 
{
  CheckLocked();  
  Backup();
  myTitle = theTitle;
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

std::string SALOMEDSImpl_AttributeTableOfReal::GetTitle() const 
{
  return myTitle;
}

void SALOMEDSImpl_AttributeTableOfReal::SetRowData(const int theRow,
						   const vector<double>& theData) 
{
  CheckLocked();  
  if(theData.size() > myNbColumns) SetNbColumns(theData.size());

  Backup();

  while (myRows.size() < theRow) { // append new row titles
    myRows.push_back(std::string(""));
  }

  int i, aShift = (theRow-1)*myNbColumns, aLength = theData.size();
  for(i = 1; i <= aLength; i++) {
    myTable[aShift + i] = theData[i-1];
  }

  if(theRow > myNbRows) myNbRows = theRow;
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

vector<double> SALOMEDSImpl_AttributeTableOfReal::GetRowData(const int theRow)
{
  vector<double> aSeq;
  int i, aShift = (theRow-1)*myNbColumns;
  for(i = 1; i <= myNbColumns; i++) {
     if(myTable.find(aShift+i) != myTable.end()) 
       aSeq.push_back(myTable[aShift+i]);
     else
       aSeq.push_back(0.);
  }
  
  return aSeq;
}


void SALOMEDSImpl_AttributeTableOfReal::SetRowTitle(const int theRow,
						    const std::string& theTitle) 
{
  CheckLocked();  
  Backup();
  std::string aTitle(theTitle), aUnit = GetRowUnit(theRow);
  if(aUnit.size()>0) {
    aTitle += SEPARATOR;
    aTitle += aUnit;
  }
  myRows[theRow-1] = aTitle;
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

void SALOMEDSImpl_AttributeTableOfReal::SetRowUnit(const int theRow,
						   const std::string& theUnit) 
{
  CheckLocked();  
  Backup();
  std::string aTitle = GetRowTitle(theRow);
  aTitle += SEPARATOR;
  aTitle += theUnit;
  
  myRows[theRow-1] = aTitle;
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

void SALOMEDSImpl_AttributeTableOfReal::SetRowUnits(const vector<string>& theUnits)
{
  if (theUnits.size() != GetNbRows()) throw DFexception("Invalid number of rows");
  int aLength = theUnits.size(), i;
  for(i = 1; i <= aLength; i++) SetRowUnit(i, theUnits[i-1]);
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

vector<string> SALOMEDSImpl_AttributeTableOfReal::GetRowUnits()
{
  vector<string> aSeq;
  int aLength = myRows.size(), i;
  for(i=0; i<aLength; i++) aSeq.push_back(getUnit(myRows[i]));
  return aSeq;
}

void SALOMEDSImpl_AttributeTableOfReal::SetRowTitles(const vector<string>& theTitles)
{
  if (theTitles.size() != GetNbRows()) throw DFexception("Invalid number of rows");
  int aLength = theTitles.size(), i;
  for(i = 1; i <= aLength; i++) SetRowTitle(i, theTitles[i-1]);
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

vector<string> SALOMEDSImpl_AttributeTableOfReal::GetRowTitles()
{
  vector<string> aSeq;
  int aLength = myRows.size(), i;
  for(i=0; i<aLength; i++) aSeq.push_back(getTitle(myRows[i]));
  return aSeq;
}


std::string SALOMEDSImpl_AttributeTableOfReal::GetRowTitle(const int theRow) const 
{
  return getTitle(myRows[theRow-1]);
}


std::string SALOMEDSImpl_AttributeTableOfReal::GetRowUnit(const int theRow) const 
{
  return getUnit(myRows[theRow-1]);
}

void SALOMEDSImpl_AttributeTableOfReal::SetColumnData(const int theColumn,
						      const vector<double>& theData) 
{
  CheckLocked();  
  if(theColumn > myNbColumns) SetNbColumns(theColumn);

  Backup();

  int i, aLength = theData.size();
  for(i = 1; i <= aLength; i++) {
    myTable[myNbColumns*(i-1)+theColumn] = theData[i-1];
  }

  if(aLength > myNbRows) {
    myNbRows = aLength;
    while (myRows.size() < myNbRows) { // append empty row titles
      myRows.push_back(string(""));
    }
  }
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}


vector<double> SALOMEDSImpl_AttributeTableOfReal::GetColumnData(const int theColumn)
{
  vector<double> aSeq;
  
  int i, anIndex;
  for(i = 1; i <= myNbRows; i++) {
    anIndex = myNbColumns*(i-1) + theColumn;
    if(myTable.find(anIndex) != myTable.end()) 
      aSeq.push_back(myTable[anIndex]);
    else
      aSeq.push_back(0.);
  }
  
  return aSeq;
}

void SALOMEDSImpl_AttributeTableOfReal::SetColumnTitle(const int theColumn,
						       const std::string& theTitle) 
{
  CheckLocked();  
  Backup();
  while(myCols.size() < theColumn) myCols.push_back(std::string(""));
  myCols[theColumn-1] = theTitle;

  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

std::string SALOMEDSImpl_AttributeTableOfReal::GetColumnTitle(const int theColumn) const 
{
  if(myCols.empty()) return "";
  if(myCols.size() < theColumn) return "";
  return myCols[theColumn-1];
}

void SALOMEDSImpl_AttributeTableOfReal::SetColumnTitles(const vector<string>& theTitles)
{
  if (theTitles.size() != myNbColumns) throw DFexception("Invalid number of columns");
  int aLength = theTitles.size(), i;
  for(i = 0; i < aLength; i++)  myCols[i] = theTitles[i];
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

vector<string> SALOMEDSImpl_AttributeTableOfReal::GetColumnTitles()
{
  vector<string> aSeq;
  int aLength = myCols.size(), i;
  for(i=0; i<aLength; i++) aSeq.push_back(myCols[i]);
  return aSeq;
}

int SALOMEDSImpl_AttributeTableOfReal::GetNbRows() const
{
  return myNbRows;
}

int SALOMEDSImpl_AttributeTableOfReal::GetNbColumns() const
{
  return myNbColumns;
}

void SALOMEDSImpl_AttributeTableOfReal::PutValue(const double& theValue,
					         const int theRow,
					         const int theColumn) 
{
  CheckLocked();      
  if(theColumn > myNbColumns) SetNbColumns(theColumn);

  int anIndex = (theRow-1)*myNbColumns + theColumn;
  myTable[anIndex] =  theValue;

  if(theRow > myNbRows) {
    while (myRows.size() < theRow) { // append empty row titles
      myRows.push_back(std::string(""));
    }
    myNbRows = theRow;
  }
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

bool SALOMEDSImpl_AttributeTableOfReal::HasValue(const int theRow,
						 const int theColumn) 
{
  if(theRow > myNbRows || theRow < 1) return false;
  if(theColumn > myNbColumns || theColumn < 1) return false;
  int anIndex = (theRow-1)*myNbColumns + theColumn;
  return (myTable.find(anIndex) != myTable.end()); 
}

double SALOMEDSImpl_AttributeTableOfReal::GetValue(const int theRow,
						   const int theColumn) 
{
  if(theRow > myNbRows || theRow < 1) throw DFexception("Invalid cell index");
  if(theColumn > myNbColumns || theColumn < 1) throw DFexception("Invalid cell index");

  int anIndex = (theRow-1)*myNbColumns + theColumn;
  if(myTable.find(anIndex) != myTable.end()) return myTable[anIndex];
  
  throw DFexception("Invalid cell index");
  return 0.;
}

const std::string& SALOMEDSImpl_AttributeTableOfReal::ID() const
{
  return GetID();
}

void SALOMEDSImpl_AttributeTableOfReal::Restore(DF_Attribute* with) 
{
  int anIndex;
  SALOMEDSImpl_AttributeTableOfReal* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(with);
  if(!aTable) throw DFexception("Can't Restore from a null attribute");

  myTable.clear();
  myCols.clear();
  myRows.clear();

  myTable = aTable->myTable;
  myNbRows = aTable->myNbRows;
  myNbColumns = aTable->myNbColumns;
  myTitle = aTable->myTitle;
  
  for(anIndex = 1; anIndex <= aTable->GetNbRows();anIndex++)
    myRows.push_back(aTable->GetRowTitle(anIndex));

  for(anIndex = 1; anIndex <= aTable->GetNbColumns(); anIndex++) 
    myCols.push_back(aTable->GetColumnTitle(anIndex));
}

DF_Attribute* SALOMEDSImpl_AttributeTableOfReal::NewEmpty() const
{
  return new SALOMEDSImpl_AttributeTableOfReal();
}

void SALOMEDSImpl_AttributeTableOfReal::Paste(DF_Attribute* into)
{
  int anIndex;
  SALOMEDSImpl_AttributeTableOfReal* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(into);
  if(!aTable) throw DFexception("Can't Paste into a null attribute"); 

  aTable->myTable.clear();
  aTable->myCols.clear();
  aTable->myRows.clear();

  aTable->myTable = myTable;
  aTable->myTitle = myTitle;
  aTable->myNbRows = myNbRows;
  aTable->myNbColumns = myNbColumns;

  for(anIndex = 1; anIndex <= GetNbRows();anIndex++)
    aTable->myRows.push_back(GetRowTitle(anIndex));
  for(anIndex = 1; anIndex <= GetNbColumns(); anIndex++) 
    aTable->myCols.push_back(GetColumnTitle(anIndex));
}


vector<int> SALOMEDSImpl_AttributeTableOfReal::GetSetRowIndices(const int theRow)
{
  vector<int> aSeq;

  int i, aShift = myNbColumns*(theRow-1);
  for(i = 1; i <= myNbColumns; i++) {
    if(myTable.find(aShift + i) != myTable.end()) aSeq.push_back(i);
  }
  
  return aSeq;
}

vector<int> SALOMEDSImpl_AttributeTableOfReal::GetSetColumnIndices(const int theColumn)
{
  vector<int> aSeq;

  int i, anIndex;
  for(i = 1; i <= myNbRows; i++) {
    anIndex = myNbColumns*(i-1)+theColumn;
    if(myTable.find(anIndex) != myTable.end()) aSeq.push_back(i);
  }
  
  return aSeq;
}

string SALOMEDSImpl_AttributeTableOfReal::Save() 
{
  ostrstream theStream;
  int i, j, l;

  //Title
  l = myTitle.size();
  theStream << l << "\n";
  for(i=0; i<l; i++)
    theStream << myTitle[i] << "\n";

  //Nb rows
  theStream << myNbRows << "\n";

  //Rows titles
  for(i=0; i<myNbRows; i++) {
    l = myRows[i].size();
    theStream << l << "\n";
    for(j=0; j<l; j++)
      theStream << myRows[i][j] << "\n";
  }

  //Nb columns
  theStream << myNbColumns << "\n";

  //Columns titles
  for(i=0; i<myNbColumns; i++) {
    l = myCols[i].size();
    theStream << l << "\n";
    for(j=0; j<l; j++)
      theStream << myCols[i][j] << "\n";
  }

  //Store the table values
  l = myTable.size();
  theStream << l << "\n";
  char *aBuffer = new char[128];
  for(MI p = myTable.begin(); p != myTable.end(); p++) {
    theStream << p->first << "\n";
    sprintf(aBuffer, "%.64e", p->second);
    theStream << aBuffer << "\n";
  }
  
  delete []aBuffer;
  string aString((char*)theStream.rdbuf()->str());
  return aString;
}

void SALOMEDSImpl_AttributeTableOfReal::Load(const string& value) 
{
  istrstream theStream(value.c_str(), strlen(value.c_str()));
  Backup();

  int i, j, l;

  char anExtChar;
  std::string aStr;

  //Title
  theStream >> l;

  myTitle = std::string(l, 0);
  for(i=0; i<l; i++) {
    theStream >> anExtChar;
    myTitle[i] = anExtChar;
  }

  //Nb rows
  theStream >> myNbRows;

  //Rows titles
  myRows.clear();  
  for(i=1; i<=myNbRows; i++) { 
    theStream >> l;
    aStr = std::string(l,0);
    for(j=0; j<l; j++) {
      theStream >> anExtChar;
      aStr[j] = anExtChar;
    }
    myRows.push_back(aStr);
  }

  //Nb columns
  theStream >> myNbColumns;

  //Columns titles
  myCols.clear();
  for(i=1; i<=myNbColumns; i++) {
    theStream >> l;
    aStr = std::string(l,0);
    for(j=0; j<l; j++) {
      theStream >> anExtChar;
      aStr[j] = anExtChar;
    }
    myCols.push_back(aStr);
  }

  //Restore the table values
  theStream >> l;
  myTable.clear();
  for(i=1; i<=l; i++) {
    int aKey;
    double aValue;
    theStream >> aKey;
    theStream >> aValue;
    myTable[aKey] = aValue;
  }

}
