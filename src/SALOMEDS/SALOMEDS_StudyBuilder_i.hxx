//  File      : SALOMEDS_StudyBuilder_i.hxx
//  Created   : Wed Nov 28 16:26:50 2001
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2001
//  $Header$

#ifndef __SALOMEDS_STUDYBUIlDER_I_H__
#define __SALOMEDS_STUDYBUILDER_I_H__


// std C++ headers
#include <iostream.h>

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include "SALOMEDS_AttributeComment_i.hxx"
#include "SALOMEDS_AttributePersistentRef_i.hxx"
#include "SALOMEDS_AttributeIOR_i.hxx"
#include "SALOMEDS_AttributeReal_i.hxx"
#include "SALOMEDS_AttributeInteger_i.hxx"
#include "SALOMEDS_AttributeSequenceOfInteger_i.hxx"
#include "SALOMEDS_AttributeName_i.hxx"
#include "SALOMEDS_AttributeSequenceOfReal_i.hxx"
#include "SALOMEDS_Callback_i.hxx"

// Cascade header
#include <TDocStd_Document.hxx>

class SALOMEDS_StudyBuilder_i: public POA_SALOMEDS::StudyBuilder,
			  public PortableServer::RefCountServantBase {
private:
  CORBA::ORB_ptr           _orb;
  Handle(TDocStd_Document) _doc;  // OCAF Document
  SALOMEDS::Callback_var   _callbackOnAdd;
  SALOMEDS::Callback_var   _callbackOnRemove;
public:
    
  SALOMEDS_StudyBuilder_i(const Handle(TDocStd_Document), CORBA::ORB_ptr);

  ~SALOMEDS_StudyBuilder_i();

  //! NewComponent
  /*!
    \param ComponentDataType    
    \return <ReturnValue>
  */
  virtual SALOMEDS::SComponent_ptr NewComponent(const char* ComponentDataType);

  //! DefineComponentInstance
  /*!
    \param SComponent_ptr       
    \param ComponentIOR	
  */
  virtual void DefineComponentInstance (SALOMEDS::SComponent_ptr, CORBA::Object_ptr ComponentIOR);
  
  //! 
  /*!
    <long-description>

    \param aComponent	
  */
  virtual void RemoveComponent(SALOMEDS::SComponent_ptr aComponent);

  //! 
  /*!
    <long-description>

    \param theFatherObject      
    \return <ReturnValue>
  */
  virtual SALOMEDS::SObject_ptr NewObject(SALOMEDS::SObject_ptr theFatherObject);
  //! 
  /*!
    <long-description>

    \param theFatherObject      
    \param atag 
    \return <ReturnValue>
  */
  virtual SALOMEDS::SObject_ptr NewObjectToTag(SALOMEDS::SObject_ptr theFatherObject, CORBA::Long atag);

  /*!
    The methods adds a new subdirectory, the path can be absolute or relative (then the current context is used)
  */
  virtual void AddDirectory(const char* thePath);

  virtual void LoadWith(SALOMEDS::SComponent_ptr sco, SALOMEDS::Driver_ptr Engine)
    throw(SALOME::SALOME_Exception);
  virtual void Load(SALOMEDS::SObject_ptr sco);

  virtual void RemoveObject(SALOMEDS::SObject_ptr anObject);
  virtual void RemoveObjectWithChildren(SALOMEDS::SObject_ptr anObject);

  virtual SALOMEDS::GenericAttribute_ptr FindOrCreateAttribute(SALOMEDS::SObject_ptr anObject, const char* aTypeOfAttribute);
  virtual CORBA::Boolean FindAttribute(SALOMEDS::SObject_ptr anObject, SALOMEDS::GenericAttribute_out anAttribute, const char* aTypeOfAttribute);
  virtual void RemoveAttribute(SALOMEDS::SObject_ptr anObject, const char* aTypeOfAttribute);

  virtual void Addreference(SALOMEDS::SObject_ptr me, SALOMEDS::SObject_ptr thereferencedObject);

  virtual void SetGUID(SALOMEDS::SObject_ptr anObject, const char* theGUID);
  virtual bool IsGUID(SALOMEDS::SObject_ptr anObject, const char* theGUID);

  virtual void NewCommand();
  virtual void CommitCommand() throw(SALOMEDS::StudyBuilder::LockProtection);
  virtual CORBA::Boolean HasOpenCommand();
  virtual void AbortCommand();
  virtual void Undo() throw(SALOMEDS::StudyBuilder::LockProtection);
  virtual void Redo() throw(SALOMEDS::StudyBuilder::LockProtection);
  CORBA::Boolean GetAvailableUndos();
  CORBA::Boolean GetAvailableRedos();
  CORBA::Boolean IsSaved();
  CORBA::Boolean IsModified();
  virtual CORBA::Long UndoLimit();
  virtual void UndoLimit(CORBA::Long);

  virtual SALOMEDS::Callback_ptr SetOnAddSObject(SALOMEDS::Callback_ptr theCallback);
  virtual SALOMEDS::Callback_ptr SetOnRemoveSObject(SALOMEDS::Callback_ptr theCallback);
};
#endif
