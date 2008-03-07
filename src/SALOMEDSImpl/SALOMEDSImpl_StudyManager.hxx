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
//  File   : SALOMEDSImpl_StudyManager.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDSImpl_STUDYMANAGER_I_H__
#define __SALOMEDSImpl_STUDYMANAGER_I_H__

// std C++ headers
#include <strstream>
#include <string>
#include <vector>

#include "DF_Application.hxx"
#include "SALOMEDSImpl_Study.hxx"
#include "SALOMEDSImpl_SObject.hxx"
#include "SALOMEDSImpl_Driver.hxx"
#include "DF_Attribute.hxx"
#include "DF_Label.hxx"
#include "DF_Document.hxx"

class HDFgroup;

class SALOMEDSImpl_StudyManager
{

private:

  DF_Application*   _appli;  
  int               _IDcounter;
  DF_Document*      _clipboard;
  std::string       _errorCode;

public:

  //! standard constructor
  Standard_EXPORT SALOMEDSImpl_StudyManager();

  //! standard destructor
  Standard_EXPORT virtual  ~SALOMEDSImpl_StudyManager(); 

  //! method to Create a New Study of name study_name
  Standard_EXPORT virtual SALOMEDSImpl_Study* NewStudy(const std::string& study_name);

  //! method to Open a Study from it's persistent reference
  Standard_EXPORT virtual SALOMEDSImpl_Study* Open(const std::string& aStudyUrl);

  //! method to close a Study 
  Standard_EXPORT virtual void Close(SALOMEDSImpl_Study* aStudy);

  //! method to save a Study 
  Standard_EXPORT virtual bool Save(SALOMEDSImpl_Study* aStudy, SALOMEDSImpl_DriverFactory* aFactory, bool theMultiFile);

  Standard_EXPORT virtual bool SaveASCII(SALOMEDSImpl_Study* aStudy, 
			                 SALOMEDSImpl_DriverFactory* aFactory, 
			                 bool theMultiFile);

  //! method to save a Study to the persistent reference aUrl
  Standard_EXPORT virtual bool SaveAs(const std::string& aUrl,  
		                      SALOMEDSImpl_Study* aStudy, 
		                      SALOMEDSImpl_DriverFactory* aFactory,
		                      bool theMultiFile);

  Standard_EXPORT virtual bool SaveAsASCII(const std::string& aUrl, 
			                   SALOMEDSImpl_Study* aStudy, 
			                   SALOMEDSImpl_DriverFactory* aFactory,
			                   bool theMultiFile);

  //! method to Get name list of open studies in the session
  Standard_EXPORT virtual std::vector<SALOMEDSImpl_Study*> GetOpenStudies();

  //! method to get a Study from it's name
  Standard_EXPORT virtual SALOMEDSImpl_Study* GetStudyByName(const std::string& aStudyName) ;

  //! method to get a Study from it's ID
  Standard_EXPORT virtual SALOMEDSImpl_Study* GetStudyByID(int aStudyID) ;


  Standard_EXPORT DF_Document* GetDocumentOfStudy(SALOMEDSImpl_Study* theStudy);

  Standard_EXPORT DF_Document* GetClipboard() { return _clipboard; }
  
  Standard_EXPORT bool CopyLabel(SALOMEDSImpl_Study* theSourceStudy, 
		                 SALOMEDSImpl_Driver* theEngine,
		                 const int theSourceStartDepth,
		                 const DF_Label& theSource,
		                 const DF_Label& theDestinationMain);

  Standard_EXPORT DF_Label PasteLabel(SALOMEDSImpl_Study* theDestinationStudy,
		                       SALOMEDSImpl_Driver* theEngine,
		                       const DF_Label& theSource,
		                       const DF_Label& theDestinationStart,
		                       const int theCopiedStudyID,
		                       const bool isFirstElement);
  
  Standard_EXPORT virtual bool CanCopy(const SALOMEDSImpl_SObject& theObject, SALOMEDSImpl_Driver* Engine);
  Standard_EXPORT virtual bool Copy(const SALOMEDSImpl_SObject& theObject, SALOMEDSImpl_Driver* Engine);
  Standard_EXPORT virtual bool CanPaste(const SALOMEDSImpl_SObject& theObject, SALOMEDSImpl_Driver* Engine);
  Standard_EXPORT virtual SALOMEDSImpl_SObject Paste(const SALOMEDSImpl_SObject& theObject, SALOMEDSImpl_Driver* Engine);

  // _SaveAs private function called by Save and SaveAs
  Standard_EXPORT virtual bool Impl_SaveAs(const std::string& aUrl,
			                   SALOMEDSImpl_Study* aStudy,
			                   SALOMEDSImpl_DriverFactory* aFactory,
			                   bool theMultiFile,
			                   bool theASCII);

  // _SaveObject private function called by _SaveAs
  Standard_EXPORT virtual bool Impl_SaveObject(const SALOMEDSImpl_SObject& SC, HDFgroup *hdf_group_datatype);

  // _SubstituteSlash function called by Open and GetStudyByName
  Standard_EXPORT virtual std::string Impl_SubstituteSlash(const std::string& aUrl);

  Standard_EXPORT virtual bool Impl_SaveProperties(SALOMEDSImpl_Study* aStudy, HDFgroup *hdf_group);

  Standard_EXPORT std::string GetErrorCode() { return _errorCode; }
  Standard_EXPORT virtual bool IsError() { return _errorCode != ""; }

};

#endif 
