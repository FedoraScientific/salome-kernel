using namespace std;
//  File      : SALOMEDS_SequenceOfRealAttribute.cxx
//  Created   : Wed Nov 28 16:09:35 2001
//  Author    : Yves FRICAUD
//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2001
//  $Header: 

#include "SALOMEDS_SequenceOfRealAttribute.ixx"
#include <TDataStd_Real.hxx>

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDS_SequenceOfRealAttribute::GetID () 
{
  static Standard_GUID SALOMEDS_SequenceOfRealAttributeID ("12837183-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDS_SequenceOfRealAttributeID;
}



//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

Handle(SALOMEDS_SequenceOfRealAttribute) SALOMEDS_SequenceOfRealAttribute::Set (const TDF_Label& L) 
{
  Handle(SALOMEDS_SequenceOfRealAttribute) A;
  if (!L.FindAttribute(SALOMEDS_SequenceOfRealAttribute::GetID(),A)) {
    A = new  SALOMEDS_SequenceOfRealAttribute(); 
    L.AddAttribute(A);
  }
  return A;
}


//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDS_SequenceOfRealAttribute::SALOMEDS_SequenceOfRealAttribute()
{myValue = new TColStd_HSequenceOfReal();}

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDS_SequenceOfRealAttribute::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

Handle(TDF_Attribute) SALOMEDS_SequenceOfRealAttribute::NewEmpty () const
{  
  return new SALOMEDS_SequenceOfRealAttribute(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDS_SequenceOfRealAttribute::Restore(const Handle(TDF_Attribute)& with) 
{
  Standard_Integer i;
  Handle(SALOMEDS_SequenceOfRealAttribute) anSeq = Handle(SALOMEDS_SequenceOfRealAttribute)::DownCast(with);
  if(!anSeq->myValue.IsNull()) {
    myValue = new TColStd_HSequenceOfReal();
    Standard_Integer Len = anSeq->Length();
    for(i = 1; i<=Len; i++) Add(anSeq->Value(i)); 
  }
  else
    myValue.Nullify();
  return;
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDS_SequenceOfRealAttribute::Paste (const Handle(TDF_Attribute)& into,
                                    const Handle(TDF_RelocationTable)& ) const
{
  if(!myValue.IsNull()) {
    Handle(SALOMEDS_SequenceOfRealAttribute)::DownCast (into)->Assign(myValue);
  }
}

void SALOMEDS_SequenceOfRealAttribute::Assign(const Handle(TColStd_HSequenceOfReal)& other) 
{
  Backup();
  if (myValue.IsNull()) myValue = new TColStd_HSequenceOfReal;
  myValue->ChangeSequence() = other->Sequence();
}

void SALOMEDS_SequenceOfRealAttribute::ChangeValue(const Standard_Integer Index,const Standard_Real Value) 
{
  Backup();
  myValue->SetValue(Index, Value);
}

void SALOMEDS_SequenceOfRealAttribute::Add(const Standard_Real Value) 
{
  Backup();
  myValue->Append(Value);
}

void SALOMEDS_SequenceOfRealAttribute::Remove(const Standard_Integer Index) 
{
  Backup();
  myValue->Remove(Index);
}

Standard_Integer SALOMEDS_SequenceOfRealAttribute::Length() 
{
  return myValue->Length();
}

Standard_Real SALOMEDS_SequenceOfRealAttribute::Value(const Standard_Integer Index) 
{
  return myValue->Value(Index);
}

