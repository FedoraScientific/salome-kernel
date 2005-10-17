//  File   : SALOMEDSImpl_Attributes.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDSIMPL_ATTRIBUTES__H__
#define __SALOMEDSIMPL_ATTRIBUTES__H__

#include <iostream>
class LockProtection
{
public :
  LockProtection(const char *message) {
    std::cerr << message << std::endl;
  }
}; 

#include "SALOMEDSImpl_GenericAttribute.hxx"
#include "SALOMEDSImpl_AttributeName.hxx"
#include "SALOMEDSImpl_AttributeComment.hxx"
#include "SALOMEDSImpl_AttributeInteger.hxx"
#include "SALOMEDSImpl_AttributeReal.hxx"
#include "SALOMEDSImpl_AttributeTreeNode.hxx"
#include "SALOMEDSImpl_AttributeUserID.hxx"
#include "SALOMEDSImpl_AttributeReference.hxx"
#include "SALOMEDSImpl_AttributeSequenceOfReal.hxx"
#include "SALOMEDSImpl_AttributeSequenceOfInteger.hxx" 
#include "SALOMEDSImpl_AttributeIOR.hxx"
#include "SALOMEDSImpl_AttributePersistentRef.hxx" 
#include "SALOMEDSImpl_AttributeDrawable.hxx"
#include "SALOMEDSImpl_AttributeSelectable.hxx"
#include "SALOMEDSImpl_AttributeExpandable.hxx"
#include "SALOMEDSImpl_AttributeOpened.hxx"
#include "SALOMEDSImpl_AttributeTextColor.hxx"
#include "SALOMEDSImpl_AttributeTextHighlightColor.hxx"
#include "SALOMEDSImpl_AttributePixMap.hxx"
#include "SALOMEDSImpl_AttributeLocalID.hxx"
#include "SALOMEDSImpl_AttributeTarget.hxx"
#include "SALOMEDSImpl_AttributeTableOfInteger.hxx"
#include "SALOMEDSImpl_AttributeTableOfReal.hxx"
#include "SALOMEDSImpl_AttributeTableOfString.hxx"
#include "SALOMEDSImpl_AttributeStudyProperties.hxx"
#include "SALOMEDSImpl_AttributePythonObject.hxx"
#include "SALOMEDSImpl_AttributeExternalFileDef.hxx"
#include "SALOMEDSImpl_AttributeFileType.hxx"
#include "SALOMEDSImpl_AttributeFlags.hxx"
#include "SALOMEDSImpl_AttributeGraphic.hxx"


#define __AttributeTypeToGUIDForSObject \
	if(theType == "AttributeReal") return SALOMEDSImpl_AttributeReal::GetID(); \
	if(theType == "AttributeInteger") return SALOMEDSImpl_AttributeInteger::GetID(); \
	if(theType == "AttributeSequenceOfReal") return SALOMEDSImpl_AttributeSequenceOfReal::GetID(); \
	if(theType == "AttributeSequenceOfInteger") return SALOMEDSImpl_AttributeSequenceOfInteger::GetID(); \
	if(theType == "AttributeName") return SALOMEDSImpl_AttributeName::GetID(); \
        if(theType == "AttributeComment") return SALOMEDSImpl_AttributeComment::GetID(); \
        if(theType == "AttributeIOR") return SALOMEDSImpl_AttributeIOR::GetID(); \
        if(theType == "AttributePersistentRef") return SALOMEDSImpl_AttributePersistentRef::GetID(); \
        if(theType == "AttributeDrawable") return SALOMEDSImpl_AttributeDrawable::GetID(); \
        if(theType == "AttributeSelectable") return SALOMEDSImpl_AttributeSelectable::GetID(); \
	if(theType == "AttributeExpandable") return SALOMEDSImpl_AttributeExpandable::GetID(); \
	if(theType == "AttributeOpened") return SALOMEDSImpl_AttributeOpened::GetID(); \
	if(theType == "AttributeTextColor") return SALOMEDSImpl_AttributeTextColor::GetID(); \
	if(theType == "AttributeTextHighlightColor") return SALOMEDSImpl_AttributeTextHighlightColor::GetID(); \
	if(theType == "AttributePixMap") return SALOMEDSImpl_AttributePixMap::GetID(); \
	if(theType == "AttributeLocalID") return SALOMEDSImpl_AttributeLocalID::GetID(); \
	if(theType == "AttributeTarget") return SALOMEDSImpl_AttributeTarget::GetID(); \
	if(theType == "AttributeTableOfInteger") return SALOMEDSImpl_AttributeTableOfInteger::GetID(); \
	if(theType == "AttributeTableOfReal") return SALOMEDSImpl_AttributeTableOfReal::GetID();  \
        if(theType == "AttributeTableOfString") return SALOMEDSImpl_AttributeTableOfString::GetID(); \
	if(theType == "AttributeStudyProperties") return SALOMEDSImpl_AttributeStudyProperties::GetID(); \
	if(theType == "AttributePythonObject") return SALOMEDSImpl_AttributePythonObject::GetID(); \
	if(theType == "AttributeUserID") return Standard_GUID("FFFFFFFF-D9CD-11d6-945D-1050DA506788"); \
	if(theType == "AttributeExternalFileDef") return SALOMEDSImpl_AttributeExternalFileDef::GetID(); \
	if(theType == "AttributeFileType") return SALOMEDSImpl_AttributeFileType::GetID(); \
	if(theType == "AttributeFlags") return SALOMEDSImpl_AttributeFlags::GetID(); \
        if(theType == "AttributeGraphic") return SALOMEDSImpl_AttributeGraphic::GetID(); \
	if(theType == "AttributeReference") return SALOMEDSImpl_AttributeReference::GetID();


#define __FindOrCreateAttributeLocked(ClassName) if (strcmp(aTypeOfAttribute.ToCString(), #ClassName) == 0) { \
    Handle(SALOMEDSImpl_##ClassName) anAttr; \
    if (!Lab.FindAttribute(SALOMEDSImpl_##ClassName::GetID(), anAttr)) { \
      CheckLocked(); \
      anAttr = new SALOMEDSImpl_##ClassName; \
      Lab.AddAttribute(anAttr); \
    } \
    return anAttr; \
  }   

#define __FindOrCreateAttribute(ClassName) if (strcmp(aTypeOfAttribute.ToCString(), #ClassName) == 0) { \
    Handle(SALOMEDSImpl_##ClassName) anAttr; \
    if (!Lab.FindAttribute(SALOMEDSImpl_##ClassName::GetID(), anAttr)) { \
      anAttr = new SALOMEDSImpl_##ClassName; \
      Lab.AddAttribute(anAttr); \
    } \
    return anAttr; \
  }      

#define __FindOrCreateAttributeForBuilder \
__FindOrCreateAttributeLocked(AttributeReal) \
__FindOrCreateAttributeLocked(AttributeInteger) \
__FindOrCreateAttributeLocked(AttributeSequenceOfReal) \
__FindOrCreateAttributeLocked(AttributeSequenceOfInteger) \
__FindOrCreateAttributeLocked(AttributeName) \
__FindOrCreateAttributeLocked(AttributeComment) \
__FindOrCreateAttributeLocked(AttributeIOR) \
__FindOrCreateAttributeLocked(AttributePixMap) \
__FindOrCreateAttributeLocked(AttributeLocalID) \
__FindOrCreateAttributeLocked(AttributeTableOfInteger) \
__FindOrCreateAttributeLocked(AttributeTableOfReal) \
__FindOrCreateAttributeLocked(AttributeTableOfString) \
__FindOrCreateAttributeLocked(AttributePythonObject) \
__FindOrCreateAttribute(AttributePersistentRef) \
__FindOrCreateAttribute(AttributeDrawable) \
__FindOrCreateAttribute(AttributeSelectable) \
__FindOrCreateAttribute(AttributeExpandable) \
__FindOrCreateAttribute(AttributeOpened) \
__FindOrCreateAttribute(AttributeTextColor) \
__FindOrCreateAttribute(AttributeTextHighlightColor) \
__FindOrCreateAttribute(AttributeTarget) \
__FindOrCreateAttribute(AttributeStudyProperties) \
__FindOrCreateAttribute(AttributeExternalFileDef) \
__FindOrCreateAttribute(AttributeFileType) \
__FindOrCreateAttribute(AttributeFlags) \
__FindOrCreateAttribute(AttributeGraphic)


#endif