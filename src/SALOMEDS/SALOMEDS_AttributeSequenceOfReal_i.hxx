//  File      : SALOMEDS_AttributeSequenceOfSequenceOfReal_i.hxx
//  Created   : Fri Jul 05 10:57:32 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header$


#ifndef SALOMEDS_AttributeSequenceOfSequenceOfReal_i_HeaderFile
#define SALOMEDS_AttributeSequenceOfSequenceOfReal_i_HeaderFile

// IDL headers
#include "SALOMEDS_SequenceOfRealAttribute.hxx"
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)
#include "SALOMEDS_GenericAttribute_i.hxx"

class SALOMEDS_AttributeSequenceOfReal_i: public virtual POA_SALOMEDS::AttributeSequenceOfReal,
					  public virtual SALOMEDS_GenericAttribute_i {
public:
  
  SALOMEDS_AttributeSequenceOfReal_i(const Handle(SALOMEDS_SequenceOfRealAttribute)& theSequenceOfRealAttr, 
				     CORBA::ORB_ptr orb) 
  {
    _myOrb = CORBA::ORB::_duplicate(orb);
    _myAttr = theSequenceOfRealAttr;
  };
  ~SALOMEDS_AttributeSequenceOfReal_i() {};

  void Assign(const SALOMEDS::DoubleSeq& other);
  SALOMEDS::DoubleSeq* CorbaSequence();
  void Add(CORBA::Double value);
  void Remove(CORBA::Long index);
  void ChangeValue(CORBA::Long index, CORBA::Double value);
  CORBA::Double Value(CORBA::Short index);
  CORBA::Long Length();
};



#endif
