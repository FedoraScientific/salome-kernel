using namespace std;
//  File      : SALOMEDS_AttributeStudyProperties_i.cxx
//  Created   : Tue Oct  8 10:13:30 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header$


#include "SALOMEDS_AttributeStudyProperties_i.hxx"
#include "SALOMEDS_SObject_i.hxx"
#include <TColStd_HSequenceOfExtendedString.hxx>
#include <TColStd_HSequenceOfInteger.hxx>

#define CREATION_MODE_NOTDEFINED 0
#define CREATION_MODE_SCRATCH 1
#define CREATION_MODE_COPY 2

void SALOMEDS_AttributeStudyProperties_i::SetUserName(const char* theName) {
  CheckLocked();
  Handle(SALOMEDS_StudyPropertiesAttribute) aProp = Handle(SALOMEDS_StudyPropertiesAttribute)::DownCast(_myAttr);
  CORBA::String_var Str = CORBA::string_dup(theName);
  aProp->SetFirstName(TCollection_ExtendedString(Str));
}

char* SALOMEDS_AttributeStudyProperties_i::GetUserName() {
  TCollection_ExtendedString S = Handle(SALOMEDS_StudyPropertiesAttribute)::DownCast(_myAttr)->GetCreatorName();
  CORBA::String_var c_s = CORBA::string_dup(TCollection_AsciiString(S).ToCString());
  return c_s._retn();
}

void SALOMEDS_AttributeStudyProperties_i::SetCreationDate(CORBA::Long theMinute,
							  CORBA::Long theHour,
							  CORBA::Long theDay,
							  CORBA::Long theMonth,
							  CORBA::Long theYear) {
  CheckLocked();
  Handle(SALOMEDS_StudyPropertiesAttribute) aProp = Handle(SALOMEDS_StudyPropertiesAttribute)::DownCast(_myAttr);
  int aTmp;
  if (aProp->GetCreationDate(aTmp, aTmp, aTmp, aTmp, aTmp)) return;
  aProp->SetModificationDate(theMinute, theHour, theDay, theMonth, theYear);
}

CORBA::Boolean SALOMEDS_AttributeStudyProperties_i::GetCreationDate(CORBA::Long& theMinute,
								    CORBA::Long& theHour,
								    CORBA::Long& theDay,
								    CORBA::Long& theMonth,
								    CORBA::Long& theYear) {
  Standard_Integer aMinute;
  Standard_Integer aHour;
  Standard_Integer aDay;
  Standard_Integer aMonth;
  Standard_Integer aYear;
  if (Handle(SALOMEDS_StudyPropertiesAttribute)::DownCast(_myAttr)->GetCreationDate(aMinute, aHour, aDay,
										    aMonth, aYear)) {
    theMinute = aMinute;
    theHour = aHour;
    theDay = aDay;
    theMonth = aMonth;
    theYear = aYear;
    return Standard_True;
  }
  return Standard_False;
}

void SALOMEDS_AttributeStudyProperties_i::SetCreationMode(const char* theMode) {
  CheckLocked();
  Handle(SALOMEDS_StudyPropertiesAttribute) aProp = Handle(SALOMEDS_StudyPropertiesAttribute)::DownCast(_myAttr);
  if (strcmp(theMode,"from scratch")==0) aProp->SetCreationMode(CREATION_MODE_SCRATCH);
  else if (strcmp(theMode,"copy from")==0) aProp->SetCreationMode(CREATION_MODE_COPY);
  else aProp->SetCreationMode(CREATION_MODE_NOTDEFINED);
}

char* SALOMEDS_AttributeStudyProperties_i::GetCreationMode() {
  CORBA::String_var c_s;
  switch (Handle(SALOMEDS_StudyPropertiesAttribute)::DownCast(_myAttr)->GetCreationMode()) {
  case CREATION_MODE_SCRATCH: c_s = "from scratch"; break;
  case CREATION_MODE_COPY: c_s = "copy from"; break;
  default: c_s = "";
  }
  return c_s._retn();
}

void SALOMEDS_AttributeStudyProperties_i::SetModified(CORBA::Long theModified) {
  Handle(SALOMEDS_StudyPropertiesAttribute)::DownCast(_myAttr)->SetModified(theModified);
}

CORBA::Boolean SALOMEDS_AttributeStudyProperties_i::IsModified() {
  return Handle(SALOMEDS_StudyPropertiesAttribute)::DownCast(_myAttr)->IsModified();
}

CORBA::Long SALOMEDS_AttributeStudyProperties_i::GetModified() {
  return Handle(SALOMEDS_StudyPropertiesAttribute)::DownCast(_myAttr)->GetModified();
}

void SALOMEDS_AttributeStudyProperties_i::SetLocked(CORBA::Boolean theLocked) {
  Handle(SALOMEDS_StudyPropertiesAttribute)::DownCast(_myAttr)->SetLocked(theLocked);
}

CORBA::Boolean SALOMEDS_AttributeStudyProperties_i::IsLocked() {
  return Handle(SALOMEDS_StudyPropertiesAttribute)::DownCast(_myAttr)->IsLocked();
}

void SALOMEDS_AttributeStudyProperties_i::SetModification(const char* theName,
							  CORBA::Long theMinute,
							  CORBA::Long theHour,
							  CORBA::Long theDay,
							  CORBA::Long theMonth,
							  CORBA::Long theYear) {
  CheckLocked();
  Handle(SALOMEDS_StudyPropertiesAttribute) aProp = Handle(SALOMEDS_StudyPropertiesAttribute)::DownCast(_myAttr);
  aProp->SetUserName(CORBA::string_dup(theName));
  aProp->SetModificationDate((int)theMinute, (int)theHour, (int)theDay, (int)theMonth, (int)theYear);
}
void SALOMEDS_AttributeStudyProperties_i::GetModificationsList(SALOMEDS::StringSeq_out theNames,
							       SALOMEDS::LongSeq_out theMinutes,
							       SALOMEDS::LongSeq_out theHours,
							       SALOMEDS::LongSeq_out theDays,
							       SALOMEDS::LongSeq_out theMonths,
							       SALOMEDS::LongSeq_out theYears,
							       CORBA::Boolean theWithCreator) {
  Handle(TColStd_HSequenceOfExtendedString) aNames;
  Handle(TColStd_HSequenceOfInteger) aMinutes, aHours, aDays, aMonths, aYears;
  Handle(SALOMEDS_StudyPropertiesAttribute) aProp = Handle(SALOMEDS_StudyPropertiesAttribute)::DownCast(_myAttr);
  aNames = aProp->GetUserNames();
  aProp->GetModificationDates(aMinutes, aHours, aDays, aMonths, aYears);
  int aLength = aNames->Length()-((theWithCreator)?0:1);
  theNames = new SALOMEDS::StringSeq;
//   SALOMEDS::LongSeq *maminute = new SALOMEDS::LongSeq;
//   maminute->length(5);
  theMinutes = new SALOMEDS::LongSeq;
  theHours = new SALOMEDS::LongSeq;
  theDays = new SALOMEDS::LongSeq;
  theMonths = new SALOMEDS::LongSeq;
  theYears = new SALOMEDS::LongSeq;
  theNames->length(aLength);
  theMinutes->length(aLength);
  theHours->length(aLength);
  theDays->length(aLength);
  theMonths->length(aLength);
  theYears->length(aLength);
  int a;
  for(a = 0; a < aLength; a++) 
    {
      (*theNames)[a] = CORBA::string_dup(TCollection_AsciiString(aNames->Value(a + 1 + ((theWithCreator)?0:1))).ToCString());
      (*theMinutes)[a] = aMinutes->Value(a + 1 + ((theWithCreator)?0:1));
      (*theHours)[a] = aHours->Value(a + 1 + ((theWithCreator)?0:1));
      (*theDays)[a] = aDays->Value(a + 1 + ((theWithCreator)?0:1));
      (*theMonths)[a] = aMonths->Value(a + 1 + ((theWithCreator)?0:1));
      (*theYears)[a] = aYears->Value(a + 1 + ((theWithCreator)?0:1));
    }
}
