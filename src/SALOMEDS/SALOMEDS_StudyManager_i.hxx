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
//  File   : SALOMEDS_StudyManager_i.hxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header$

#ifndef __SALOMEDS_STUDYMANAGER_I_H__
#define __SALOMEDS_STUDYMANAGER_I_H__

// std C++ headers
#include <iostream.h>

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)

// Cascade headers
#include "SALOMEDS_OCAFApplication.hxx"
#include <TDocStd_Document.hxx>
#include <TDF_Attribute.hxx>
#include <TDataStd_Name.hxx>
#include <TDF_Label.hxx>
#include <TDocStd_Document.hxx>

// Naming Service header
#include "SALOME_NamingService.hxx"

// HDF
#include <iostream.h>
#include "HDFOI.hxx"
#include <stdlib.h>

//Standard not implemented
#include <Standard_NotImplemented.hxx>


class SALOMEDS_StudyManager_i: public POA_SALOMEDS::StudyManager,
			       public PortableServer::RefCountServantBase {
private:

  CORBA::ORB_ptr _orb;
  Handle (SALOMEDS_OCAFApplication) _OCAFApp;  
  SALOME_NamingService* _name_service;
  int _IDcounter;
  Handle(TDocStd_Document) _clipboard;

  // _SaveAs private function called by Save and SaveAs
  virtual void _SaveAs(const char* aUrl,
		       SALOMEDS::Study_ptr aStudy,
		       CORBA::Boolean theMultiFile,
		       CORBA::Boolean theASCII);
  // _SaveObject private function called by _SaveAs
  virtual void _SaveObject(SALOMEDS::Study_ptr aStudy, 
			   SALOMEDS::SObject_ptr SC, 
			   HDFgroup *hdf_group_datatype);
  // _SubstituteSlash function called by Open and GetStudyByName
  virtual const char *_SubstituteSlash(const char *aUrl);

  virtual void _SaveProperties(SALOMEDS::Study_ptr aStudy, HDFgroup *hdf_group);

public:

  //! standard constructor
  SALOMEDS_StudyManager_i(CORBA::ORB_ptr orb);

  //! standard destructor
  virtual  ~SALOMEDS_StudyManager_i(); 

 //! method to Register study Manager in the naming service
  /*!
    \param char* arguments, the context to register the study manager in the NS
  */  
  void register_name(char * name);
  
 //! method to Create a New Study of name study_name
  /*!
    \param char* arguments, the new study name
    \return Study_ptr arguments
  */  
  virtual SALOMEDS::Study_ptr NewStudy(const char* study_name);

  //! method to Open a Study from it's persistent reference
  /*!
    \param char* arguments, the study URL
    \return Study_ptr arguments
  */ 
  virtual SALOMEDS::Study_ptr Open(const char* aStudyUrl) throw (SALOME::SALOME_Exception);


  //! method to close a Study 
  /*!
    \param Study_ptr arguments
  */ 
  virtual void Close( SALOMEDS::Study_ptr aStudy);

  //! method to save a Study 
  /*!
    \param Study_ptr arguments
  */
  virtual void Save( SALOMEDS::Study_ptr aStudy, CORBA::Boolean theMultiFile);

  virtual void SaveASCII( SALOMEDS::Study_ptr aStudy, CORBA::Boolean theMultiFile);

  //! method to save a Study to the persistent reference aUrl
  /*!
    \param char* arguments, the new URL of the study
    \param Study_ptr arguments
  */
  virtual void SaveAs(const char* aUrl,  SALOMEDS::Study_ptr aStudy, CORBA::Boolean theMultiFile);
  virtual void SaveAsASCII(const char* aUrl,  SALOMEDS::Study_ptr aStudy, CORBA::Boolean theMultiFile);

 //! method to Get name list of open studies in the session
  /*!
    \return ListOfOpenStudies* arguments
  */
  virtual SALOMEDS::ListOfOpenStudies* GetOpenStudies();

  //! method to get a Study from it's name
  /*!
    \param char* arguments, the study name
    \return Study_ptr arguments
  */ 
  virtual SALOMEDS::Study_ptr GetStudyByName(const char* aStudyName) ;

  //! method to get a Study from it's ID
  /*!
    \param char* arguments, the study ID
    \return Study_ptr arguments
  */ 
  virtual SALOMEDS::Study_ptr GetStudyByID(CORBA::Short aStudyID) ;


  Handle(TDocStd_Document) GetDocumentOfStudy(SALOMEDS::Study_ptr theStudy);
  
  void CopyLabel(const SALOMEDS::Study_ptr theSourceStudy, 
		 const SALOMEDS::Driver_ptr theEngine,
		 const Standard_Integer theSourceStartDepth,
		 const TDF_Label& theSource,
		 const TDF_Label& theDestinationMain);

  TDF_Label PasteLabel(const SALOMEDS::Study_ptr theDestinationStudy,
		       const SALOMEDS::Driver_ptr theEngine,
		       const TDF_Label& theSource,
		       const TDF_Label& theDestinationStart,
		       const int theCopiedStudyID,
		       const bool isFirstElement);
  
  virtual CORBA::Boolean CanCopy(SALOMEDS::SObject_ptr theObject);
  virtual CORBA::Boolean Copy(SALOMEDS::SObject_ptr theObject);
  virtual CORBA::Boolean CanPaste(SALOMEDS::SObject_ptr theObject);
  virtual SALOMEDS::SObject_ptr Paste(SALOMEDS::SObject_ptr theObject) throw(SALOMEDS::StudyBuilder::LockProtection);
  
  void ping(){};
};

#endif 
