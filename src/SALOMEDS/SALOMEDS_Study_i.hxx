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
//  File   : SALOMEDS_Study_i.hxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header$

#ifndef __SALOMEDS_STUDY_I_H__
#define __SALOMEDS_STUDY_I_H__

// std C++ headers
#include <iostream.h>

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)

// Cascade headers
#include <TDocStd_Document.hxx>
#include <TDF_Tool.hxx>
#include <TDF_Data.hxx>
#include <TDF_Label.hxx>
#include <stdio.h>
#include <TColStd_SequenceOfInteger.hxx>
#include <TColStd_SequenceOfAsciiString.hxx>

//SALOMEDS headers
#include "SALOMEDS_SComponentIterator_i.hxx"
#include "SALOMEDS_ChildIterator_i.hxx"
#include "SALOMEDS_StudyBuilder_i.hxx"
#include "SALOMEDS_SObject_i.hxx"
#include "SALOMEDS_DataMapStringLabel.hxx"
#include "SALOMEDS_UseCaseBuilder_i.hxx"

#include "SALOMEDS_Callback_i.hxx"

class SALOMEDS_Study_i: public POA_SALOMEDS::Study,
			public PortableServer::RefCountServantBase {
private:
  CORBA::ORB_ptr           _orb;
  char*                    _name;  
  Handle(TDocStd_Document) _doc;  // OCAF Document
  CORBA::Boolean           _isSaved; // True if the Study is saved
  char*                    _URL; //URL of the persistent reference of the study
  SALOMEDS::SObject_ptr    _FindObject(SALOMEDS::SObject_ptr SO,
				       const char* anObjectName,
				       bool& _find);
  SALOMEDS::SObject_ptr    _FindObjectIOR(SALOMEDS::SObject_ptr SO,
					  const char* anObjectIOR,
					  bool& _find);
  CORBA::Short             _StudyId;

  SALOMEDS_DataMapStringLabel myIORLabels;

  // data structures for postponed destroying of CORBA object functionality
  TColStd_SequenceOfAsciiString myPostponedIORs; // ordered set of IORs
  TColStd_SequenceOfInteger myNbPostponed; // number of IOR in the each transaction
  int myNbUndos; // number of current Undos, made by user

  TDF_Label                _current;
  bool                     _autoFill;  

public:

  //! standard constructor
  SALOMEDS_Study_i(const Handle(TDocStd_Document), 
		   CORBA::ORB_ptr, 
		   const char* study_name);
  
  //! standard destructor
  virtual ~SALOMEDS_Study_i(); 
  
  //! method to Get persistent reference of study (idem URL())
  /*!
    \sa URL()
    \return char* arguments, the persistent reference of the study
  */  
  virtual char* GetPersistentReference();


  //! method to Get transient reference of study
  /*!
    \return char* arguments, the transient reference of the study
  */  
  virtual char* GetTransientReference();

  //! method to detect if a study is empty
  /*!
    \return bool arguments, true if study is empty
  */  
  virtual CORBA::Boolean IsEmpty();

  //! method to Find a Component with ComponentDataType = aComponentName
  /*!
    \param aComponentName char* arguments
    \return SComponent_ptr arguments, the component found
  */  
  virtual SALOMEDS::SComponent_ptr FindComponent (const char* aComponentName);

  //! method to Find a Component Find a Component from it's ID
  /*!
    \param aComponentID char* arguments
    \return SComponent_ptr arguments, the component found
  */  
  virtual SALOMEDS::SComponent_ptr FindComponentID(const char* aComponentID);

  //! method to  Find an Object with SALOMEDS::Name = anObjectName 
  /*!
    \param anObjectName char* arguments
    \return SObject_ptr arguments, the object found
  */  
  virtual SALOMEDS::SObject_ptr FindObject(const char* anObjectName);


  //! method to Find Object(s) with SALOMEDS::Name = anObjectName in a component with ComponentDataType = aComponentName
  /*!
    \param anObjectName char* arguments
    \param aComponentName char* arguments
    \return ListOfSObject_ptr arguments, a list of objects found
  */  
  
  virtual SALOMEDS::Study::ListOfSObject* FindObjectByName( const char* anObjectName, const char* aComponentName ) ;
  
  //! method to Find an Object with ID = anObjectID 
  /*!
    \param anObjectID char* arguments
    \return SObject_ptr arguments, the object found
  */  
  virtual SALOMEDS::SObject_ptr FindObjectID(const char* anObjectID);

  //! method to Create an Object with ID = anObjectID 
  /*!
    \param anObjectID char* arguments
    \return SObject_ptr arguments, the object found
  */  
  virtual SALOMEDS::SObject_ptr CreateObjectID(const char* anObjectID);

  //! method to Find an Object with ID = anObjectIOR 
  /*!
    \param anObjectIOR char* arguments
    \return SObject_ptr arguments, the object found
  */  
  virtual SALOMEDS::SObject_ptr FindObjectIOR(const char* anObjectIOR);

  //! method to Find an Object by its path
  /*!
    \param thePath char* arguments
    \return SObject_ptr arguments, the object found
  */
  virtual SALOMEDS::SObject_ptr FindObjectByPath(const char* thePath);

  //! method to get a path of SObject
  /*!
    \param aSO  SObject_ptr arguments
    \return char* arguments, the path of the SObject
  */
  virtual char* GetObjectPath(CORBA::Object_ptr theObject);

  //! method to set a context: root ('/') is UserData component
  /*!
  */
  virtual void SetContext(const char* thePath);

  //! method to get a context
  /*!
  */
  virtual char* GetContext();  

  //! method to get all object names in the given context (or in the current context, if 'theContext' is empty)
  /*!
  */
  virtual SALOMEDS::ListOfStrings* GetObjectNames(const char* theContext);

  //! method to get all directory names in the given context (or in the current context, if 'theContext' is empty)
  /*!
  */
  virtual SALOMEDS::ListOfStrings* GetDirectoryNames(const char* theContext);

  //! method to get all file names in the given context (or in the current context, if 'theContext' is empty)
  /*!
  */
  virtual SALOMEDS::ListOfStrings* GetFileNames(const char* theContext);

  //! method to get all components names
  /*!
  */
  virtual SALOMEDS::ListOfStrings* GetComponentNames(const char* theContext);

  //! method to Create a ChildIterator from an SObject 
  /*!
    \param aSO  SObject_ptr arguments
    \return ChildIterator_ptr arguments, the created ChildIterator
  */  
  virtual SALOMEDS::ChildIterator_ptr NewChildIterator(SALOMEDS::SObject_ptr aSO);

  //! method to Create a SComponentIterator 
  /*!
    \return SComponentIterator_ptr arguments, the created SComponentIterator
  */  
  virtual SALOMEDS::SComponentIterator_ptr NewComponentIterator();

  //! method to Create a StudyBuilder
  /*!
    \return StudyBuilder_ptr arguments, the created StudyBuilder
  */  
  virtual SALOMEDS::StudyBuilder_ptr NewBuilder();
 
  //! method to get study name
  /*!
    \return char* arguments, the study name
  */
  virtual char* Name();

  //! method to set study name
  /*!
    \param name char* arguments, the study name
  */
  virtual void  Name(const char* name);

  //! method to get if study has been saved
  /*!
    \return bool arguments
  */
  virtual CORBA::Boolean IsSaved();

 //! method to set if study has been saved
  /*!
    \param save bool arguments
  */
  virtual void  IsSaved(CORBA::Boolean save);

 //! method to Detect if a Study has been modified since it has been saved
  /*!
    \return bool arguments
  */
  virtual CORBA::Boolean IsModified();

  //! method to get URL of the study (idem GetPersistentReference) 
  /*!
    \return char* arguments, the study URL 
  */
  virtual char* URL();

 //! method to set URL of the study
  /*!
    \param url char* arguments, the study URL
  */
  virtual void  URL(const char* url);

  virtual CORBA::Short StudyId();
  virtual void  StudyId(CORBA::Short id);

  static SALOMEDS::Study_ptr GetStudy(const TDF_Label theLabel, CORBA::ORB_ptr orb);

  static void IORUpdated(const Handle(SALOMEDS_IORAttribute) theAttribute, CORBA::ORB_ptr orb);

  virtual void UpdateIORLabelMap(const char* anIOR, const char* aLabel);
  
  virtual SALOMEDS::Study::ListOfSObject* FindDependances(SALOMEDS::SObject_ptr anObject);

  virtual SALOMEDS::AttributeStudyProperties_ptr SALOMEDS_Study_i::GetProperties();

  virtual char* GetLastModificationDate();

  virtual SALOMEDS::ListOfDates* GetModificationsDate();

  virtual char* ConvertObjectToIOR(CORBA::Object_ptr theObject) {return _orb->object_to_string(theObject); }
  virtual CORBA::Object_ptr ConvertIORToObject(const char* theIOR) { return _orb->string_to_object(theIOR); };

  virtual SALOMEDS::UseCaseBuilder_ptr GetUseCaseBuilder();

  virtual void Close();

  void EnableUseCaseAutoFilling(CORBA::Boolean isEnabled) { _autoFill = isEnabled; }

  // postponed destroying of CORBA object functionality
  virtual void AddPostponed(const char* theIOR);

  virtual void AddCreatedPostponed(const char* theIOR);

  virtual void RemovePostponed(const CORBA::Long theUndoLimit); // removes postponed IORs of old transaction
                                                        // if theUndoLimit==0, removes all
  virtual void UndoPostponed(const CORBA::Long theWay); // theWay = 1: resurrect objects,
                                                // theWay = -1: get back to the list of postponed
};
#endif
