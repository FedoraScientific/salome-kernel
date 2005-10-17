
#ifndef _SALOMEDSImpl_AttributeExternalFileDef_HeaderFile
#define _SALOMEDSImpl_AttributeExternalFileDef_HeaderFile

#include <Standard_DefineHandle.hxx>
#include <TDF_Attribute.hxx>
#include <Standard_GUID.hxx>
#include <TDF_Label.hxx>       
#include <TCollection_ExtendedString.hxx>
#include "SALOMEDSImpl_GenericAttribute.hxx"

class Handle(TDF_Attribute);
class Handle(TDF_RelocationTable);


DEFINE_STANDARD_HANDLE( SALOMEDSImpl_AttributeExternalFileDef, SALOMEDSImpl_GenericAttribute )

class SALOMEDSImpl_AttributeExternalFileDef : public SALOMEDSImpl_GenericAttribute 
{
private:

 TCollection_ExtendedString myString;

public:
Standard_EXPORT static const Standard_GUID& GetID() ;
Standard_EXPORT static  Handle_SALOMEDSImpl_AttributeExternalFileDef Set(const TDF_Label& label,
									 const TCollection_ExtendedString& Name);
Standard_EXPORT SALOMEDSImpl_AttributeExternalFileDef();
Standard_EXPORT void SetValue(const TCollection_ExtendedString& value); 
Standard_EXPORT TCollection_ExtendedString Value() const { return myString; }
Standard_EXPORT  const Standard_GUID& ID() const;
Standard_EXPORT   void Restore(const Handle(TDF_Attribute)& with) ;
Standard_EXPORT   Handle_TDF_Attribute NewEmpty() const;
Standard_EXPORT   void Paste(const Handle(TDF_Attribute)& into,const Handle(TDF_RelocationTable)& RT) const;
Standard_EXPORT ~SALOMEDSImpl_AttributeExternalFileDef() {}

Standard_EXPORT  virtual TCollection_AsciiString Save() { return myString; }
Standard_EXPORT  virtual void Load(const TCollection_AsciiString& theValue) { myString = theValue; } 

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_AttributeExternalFileDef )
};

#endif
