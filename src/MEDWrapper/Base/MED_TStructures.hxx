//  
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
//  File   : 
//  Author : 
//  Module : 
//  $Header$

#ifndef MED_TStructures_HeaderFile
#define MED_TStructures_HeaderFile

#include "MED_Structures.hxx"

namespace MED{

  const TInt DESC = 200;
  const TInt IDENT = 8;
  const TInt NOM = 32;
  const TInt LNOM = 80;

  //---------------------------------------------------------------
  template<TInt nPNOM, TInt nDESC, TInt nIDENT, TInt nNOM, TInt nLNOM, EVersion nV>
  struct TTNameInfo: virtual TNameInfo
  {
    TTNameInfo(const std::string& theValue = "")
    {
      myName.resize(nNOM+1);
      SetName(theValue);
    }

    virtual std::string GetName() const { 
      return GetString(0,nNOM,myName);
    }

    virtual void SetName(const std::string& theValue){
      SetString(0,nNOM,myName,theValue);
    }
  };


  //---------------------------------------------------------------
  template<TInt nPNOM, TInt nDESC, TInt nIDENT, TInt nNOM, TInt nLNOM, EVersion nV>
  struct TTMeshInfo: 
    virtual TMeshInfo, 
    virtual TTNameInfo<nPNOM,nDESC,nIDENT,nNOM,nLNOM,nV>
  {
    typedef TTNameInfo<nPNOM,nDESC,nIDENT,nNOM,nLNOM,nV> TNameInfoBase;

    TTMeshInfo(const PMeshInfo& theInfo):
      TNameInfoBase(theInfo->GetName())
    {
      myDim = theInfo->GetDim();
      myType = theInfo->GetType();
      
      myDesc.resize(nDESC+1);
      SetDesc(theInfo->GetDesc());
    }

    TTMeshInfo(TInt theDim = 0,
	       const std::string& theValue = "",
	       EMaillage theType = eNON_STRUCTURE,
	       const std::string& theDesc = ""):
      TNameInfoBase(theValue)
    {
      myDim = theDim;
      myType = theType;
      
      myDesc.resize(nDESC+1);
      SetDesc(theDesc);
    }

    virtual std::string GetDesc() const { 
      return GetString(0,nDESC,myDesc);
    }

    virtual void SetDesc(const std::string& theValue){
      SetString(0,nDESC,myDesc,theValue);
    }
  };


  //---------------------------------------------------------------
  template<TInt nPNOM, TInt nDESC, TInt nIDENT, TInt nNOM, TInt nLNOM, EVersion nV>
  struct TTFamilyInfo: 
    virtual TFamilyInfo, 
    virtual TTNameInfo<nPNOM,nDESC,nIDENT,nNOM,nLNOM,nV>
  {
    typedef TTNameInfo<nPNOM,nDESC,nIDENT,nNOM,nLNOM,nV> TNameInfoBase;

    TTFamilyInfo(const PMeshInfo& theMeshInfo, const PFamilyInfo& theInfo):
      TNameInfoBase(theInfo->GetName())
    {
      myMeshInfo = theMeshInfo;

      myId = theInfo->GetId();

      myNbGroup = theInfo->GetNbGroup();
      myGroupNames.resize(myNbGroup*nLNOM+1);
      if(myNbGroup){
	for(TInt anId = 0; anId < myNbGroup; anId++){
	  SetGroupName(anId,theInfo->GetGroupName(anId));
	}
      }

      myNbAttr = theInfo->GetNbAttr();
      myAttrId.resize(myNbAttr);
      myAttrVal.resize(myNbAttr);
      myAttrDesc.resize(myNbAttr*nDESC+1);
      if(myNbAttr){
	for(TInt anId = 0; anId < myNbAttr; anId++){
	  SetAttrDesc(anId,theInfo->GetAttrDesc(anId));
	  myAttrVal[anId] = theInfo->GetAttrVal(anId);
	  myAttrId[anId] = theInfo->GetAttrId(anId);
	}
      }
    }

    TTFamilyInfo(const PMeshInfo& theMeshInfo,
		 TInt theNbGroup = 0, 
		 TInt theNbAttr = 0,
		 TInt theId = 0,
		 const std::string& theValue = ""):
      TNameInfoBase(theValue)
    {
      myMeshInfo = theMeshInfo;

      myId = theId;

      myNbGroup = theNbGroup;
      myGroupNames.resize(theNbGroup*nLNOM+1);

      myNbAttr = theNbAttr;
      myAttrId.resize(theNbAttr);
      myAttrVal.resize(theNbAttr);
      myAttrDesc.resize(theNbAttr*nDESC+1);
    }

    TTFamilyInfo(const PMeshInfo& theMeshInfo,
		 const std::string& theValue,
		 TInt theId,
		 const TStringSet& theGroupNames, 
		 const TStringVector& theAttrDescs = TStringVector(), 
		 const TIntVector& theAttrIds = TIntVector(), 
		 const TIntVector& theAttrVals = TIntVector()):
      TNameInfoBase(theValue)
    {
      myMeshInfo = theMeshInfo;

      myId = theId;

      myNbGroup = theGroupNames.size();
      myGroupNames.resize(myNbGroup*nLNOM+1);
      if(myNbGroup){
	TStringSet::const_iterator anIter = theGroupNames.begin();
	for(TInt anId = 0; anIter != theGroupNames.end(); anIter++, anId++){
	  const std::string& aVal = *anIter;
	  SetGroupName(anId,aVal);
	}
      }

      myNbAttr = theAttrDescs.size();
      myAttrId.resize(myNbAttr);
      myAttrVal.resize(myNbAttr);
      myAttrDesc.resize(myNbAttr*nDESC+1);
      if(myNbAttr){
	for(TInt anId = 0, anEnd = theAttrDescs.size(); anId < anEnd; anId++){
	  SetAttrDesc(anId,theAttrDescs[anId]);
	  myAttrVal[anId] = theAttrVals[anId];
	  myAttrId[anId] = theAttrIds[anId];
	}
      }
    }

    virtual std::string GetGroupName(TInt theId) const { 
      return GetString(theId,nLNOM,myGroupNames);
    }

    virtual void SetGroupName(TInt theId, const std::string& theValue){
      SetString(theId,nLNOM,myGroupNames,theValue);
    }

    virtual std::string GetAttrDesc(TInt theId) const { 
      return GetString(theId,nDESC,myAttrDesc);
    }

    virtual void SetAttrDesc(TInt theId, const std::string& theValue){
      SetString(theId,nDESC,myAttrDesc,theValue);
    }
  };


  //---------------------------------------------------------------
  template<TInt nPNOM, TInt nDESC, TInt nIDENT, TInt nNOM, TInt nLNOM, EVersion nV>
  struct TTElemInfo: virtual TElemInfo
  {
    TTElemInfo(const PMeshInfo& theMeshInfo, const PElemInfo& theInfo)
    {
      myMeshInfo = theMeshInfo;
      
      myNbElem = theInfo->GetNbElem();
      myFamNum.resize(myNbElem);

      myIsElemNum = theInfo->IsElemNum();
      myElemNum.resize(myIsElemNum == eFAUX? 0: myNbElem);

      myIsElemNames = theInfo->IsElemNames();
      myElemNames.resize(myNbElem*nPNOM+1);

      if(myNbElem){
	for(TInt anId = 0; anId < myNbElem; anId++){
	  myFamNum[anId] = theInfo->GetFamNum(anId);
	}
	if(myIsElemNum == eVRAI){
	  for(TInt anId = 0; anId < myNbElem; anId++){
	    myElemNum[anId] = theInfo->GetElemNum(anId);
	  }
	}
	if(myIsElemNames == eVRAI){
	  for(TInt anId = 0; anId < myNbElem; anId++){
	    SetElemName(anId,theInfo->GetElemName(anId));
	  }
	}
      }
    }

    TTElemInfo(const PMeshInfo& theMeshInfo, 
	       TInt theNbElem,
	       EBooleen theIsElemNum = eVRAI,
	       EBooleen theIsElemNames = eVRAI)
    {
      myMeshInfo = theMeshInfo;

      myNbElem = theNbElem;
      myFamNum.resize(theNbElem);

      myIsElemNum = theIsElemNum;
      myElemNum.resize(theIsElemNum == eFAUX? 0: theNbElem);

      myIsElemNames = theIsElemNames;
      myElemNames.resize(theNbElem*nPNOM+1);
    }
    
    TTElemInfo(const PMeshInfo& theMeshInfo, 
	       const TIntVector& theFamilyNums,
	       const TIntVector& theElemNums,
	       const TStringVector& theElemNames = TStringVector())
    {
      myMeshInfo = theMeshInfo;
      
      myNbElem = theFamilyNums.size();
      myFamNum.resize(myNbElem);
      
      myIsElemNum = theElemNums.size()? eVRAI: eFAUX;
      myElemNum.resize(theElemNums.size());
      
      myIsElemNames = theElemNames.size()? eVRAI: eFAUX;
      myElemNames.resize(theElemNames.size()*nPNOM+1);
      
      if(myNbElem){
	for(TInt anId = 0; anId < myNbElem; anId++){
	  myFamNum[anId] = theFamilyNums[anId];
	}
	if(myIsElemNum == eVRAI){
	  for(TInt anId = 0; anId < myNbElem; anId++){
	    myElemNum[anId] = theElemNums[anId];
	  }
	}
	if(myIsElemNames == eVRAI){
	  for(TInt anId = 0; anId < myNbElem; anId++){
	    const std::string& aVal = theElemNames[anId];
	    SetElemName(anId,aVal);
	  }
	}
      }
    }

    virtual std::string GetElemName(TInt theId) const { 
      return GetString(theId,nPNOM,myElemNames);
    }

    virtual void SetElemName(TInt theId, const std::string& theValue){
      SetString(theId,nPNOM,myElemNames,theValue);
    }
  };


  //---------------------------------------------------------------
  template<TInt nPNOM, TInt nDESC, TInt nIDENT, TInt nNOM, TInt nLNOM, EVersion nV>
  struct TTNodeInfo: 
    virtual TNodeInfo, 
    virtual TTElemInfo<nPNOM,nDESC,nIDENT,nNOM,nLNOM,nV>
  {
    typedef TTElemInfo<nPNOM,nDESC,nIDENT,nNOM,nLNOM,nV> TElemInfoBase;

    TTNodeInfo(const PMeshInfo& theMeshInfo, const PNodeInfo& theInfo):
      TElemInfoBase(theMeshInfo,theInfo)
    {
      mySystem = theInfo->GetSystem();

      TInt aDim = theMeshInfo->GetDim();
      TInt aNbElem = theInfo->GetNbElem();
      myCoord.resize(aNbElem*aDim);
      
      for(TInt anElemId = 0; anElemId < aNbElem; anElemId++){
	for(TInt anDimId = 0; anDimId < aDim; anDimId++){
	  SetNodeCoord(anElemId,anDimId,theInfo->GetNodeCoord(anElemId,anDimId));
	}
      }

      myCoordNames.resize(aDim*nPNOM+1);
      for(TInt anId = 0; anId < aDim; anId++){
	SetCoordName(anId,theInfo->GetCoordName(anId));
      }
      
      myCoordUnits.resize(aDim*nPNOM+1);
      for(TInt anId = 0; anId < aDim; anId++){
	SetCoordUnit(anId,theInfo->GetCoordUnit(anId));
      }
    }

    TTNodeInfo(const PMeshInfo& theMeshInfo, 
	       TInt theNbElem,
	       ERepere theSystem = eCART, 
	       EBooleen theIsElemNum = eVRAI,
	       EBooleen theIsElemNames = eVRAI):
      TElemInfoBase(theMeshInfo,
		    theNbElem,
		    theIsElemNum,
		    theIsElemNames)
    {
      mySystem = theSystem;
      myCoord.resize(theNbElem*theMeshInfo->myDim);
      myCoordNames.resize(theMeshInfo->myDim*nPNOM+1);
      myCoordUnits.resize(theMeshInfo->myDim*nPNOM+1);
    }

    
    TTNodeInfo(const PMeshInfo& theMeshInfo, 
	       ERepere theSystem, 
	       const TFloatVector& theNodeCoords,
	       const TStringVector& theCoordNames,
	       const TStringVector& theCoordUnits,
	       const TIntVector& theFamilyNums,
	       const TIntVector& theElemNums,
	       const TStringVector& theElemNames = TStringVector()):
      TElemInfoBase(theMeshInfo,
		    theFamilyNums,
		    theElemNums,
		    theElemNames)
    {
      mySystem = theSystem;
      myCoord.resize(theNodeCoords.size());
      
      for(TInt anId = 0, anEnd = myCoord.size(); anId < anEnd; anId++){
	myCoord[anId] = theNodeCoords[anId];
      }

      TInt aDim = theMeshInfo->GetDim();
      myCoordNames.resize(aDim*nPNOM+1);
      for(TInt anId = 0; anId < aDim; anId++){
	SetCoordName(anId,theCoordNames[anId]);
      }
      
      myCoordUnits.resize(aDim*nPNOM+1);
      for(TInt anId = 0; anId < aDim; anId++){
	SetCoordUnit(anId,theCoordUnits[anId]);
      }
    }

    virtual std::string GetCoordName(TInt theId) const { 
      return GetString(theId,nPNOM,myCoordNames);
    }

    virtual void SetCoordName(TInt theId, const std::string& theValue){
      SetString(theId,nPNOM,myCoordNames,theValue);
    }

    virtual std::string GetCoordUnit(TInt theId) const { 
      return GetString(theId,nPNOM,myCoordUnits);
    }

    virtual void SetCoordUnit(TInt theId, const std::string& theValue){
      SetString(theId,nPNOM,myCoordUnits,theValue);
    }
  };

  //---------------------------------------------------------------
  template<TInt nPNOM, TInt nDESC, TInt nIDENT, TInt nNOM, TInt nLNOM, EVersion nV>
  struct TTPolygoneInfo: 
    virtual TPolygoneInfo, 
    virtual TTElemInfo<nPNOM,nDESC,nIDENT,nNOM,nLNOM,nV>
  {
    typedef TTElemInfo<nPNOM,nDESC,nIDENT,nNOM,nLNOM,nV> TElemInfoBase;

    TTPolygoneInfo(const PMeshInfo& theMeshInfo, const PPolygoneInfo& theInfo):
      TElemInfoBase(theMeshInfo,theInfo)
    {
      myTEntity = theInfo->GetEntity();
      myTGeom = theInfo->GetGeom();
      myTConn  = theInfo->GetConn();
      myConnDim = theInfo->GetConnDim();
      myConn = theInfo->GetConnectivite();
      myIndex = theInfo->GetIndex();
    }

    TTPolygoneInfo(const PMeshInfo& theMeshInfo, 
		   TInt theNbElem,
		   TInt theNbConn,
		   EEntiteMaillage theTEntity, 
		   EGeometrieElement theTGeom,
		   EConnectivite theTConn = eNOD,
		   EBooleen theIsElemNum = eVRAI,
		   EBooleen theIsElemNames = eVRAI):
      TElemInfoBase(theMeshInfo,
		    theNbElem,
		    theIsElemNum,
		    theIsElemNames)
    {
      myTEntity = theTEntity;
      myTGeom = theTGeom;
      myTConn  = theTConn;
      myConnDim = theNbConn;
      myConn.resize(myConnDim);
      myIndex.resize(theNbElem+1);
    }
    
    TTPolygoneInfo(const PMeshInfo& theMeshInfo, 
		   EEntiteMaillage theTEntity, 
		   EGeometrieElement theTGeom,
		   EConnectivite theTConn,
		   const TIntVector& theConnectivities,
		   const TIntVector& theIndexes,
		   const TIntVector& theFamilyNums,
		   const TIntVector& theElemNums,
		   const TStringVector& theElemNames = TStringVector()):
      TElemInfoBase(theMeshInfo,
		    theFamilyNums,
		    theElemNums,
		    theElemNames)
    {
      myTEntity = theTEntity;
      myTGeom = theTGeom;
      myTConn  = theTConn;
      myConnDim = theConnectivities.size();
      myConn = theConnectivities;
      myIndex = theIndexes;
    }
  };
  
  //---------------------------------------------------------------
  template<TInt nPNOM, TInt nDESC, TInt nIDENT, TInt nNOM, TInt nLNOM, EVersion nV>
  struct TTPolyedreInfo: 
    virtual TPolyedreInfo, 
    virtual TTElemInfo<nPNOM,nDESC,nIDENT,nNOM,nLNOM,nV>
  {
    typedef TTElemInfo<nPNOM,nDESC,nIDENT,nNOM,nLNOM,nV> TElemInfoBase;

    TTPolyedreInfo(const PMeshInfo& theMeshInfo, const PPolyedreInfo& theInfo):
      TElemInfoBase(theMeshInfo,theInfo)
    {
      myTEntity = theInfo->GetEntity();
      myTGeom = theInfo->GetGeom();
      myTConn  = theInfo->GetConn();
      myNbConn = theInfo->GetNbConn();
      myNbFacesIndex = theInfo->GetNbFacesIndex();
      myConn = theInfo->GetConnectivite();
      myFacesIndex = theInfo->GetFacesIndex();
      myIndex = theInfo->GetIndex();
    }

    TTPolyedreInfo(const PMeshInfo& theMeshInfo, 
		   TInt theNbElem,
		   TInt theNbConn,
		   TInt theNbFacesIndex,
		   EEntiteMaillage theTEntity, 
		   EGeometrieElement theTGeom,
		   EConnectivite theTConn = eNOD,
		   EBooleen theIsElemNum = eVRAI,
		   EBooleen theIsElemNames = eVRAI):
      TElemInfoBase(theMeshInfo,
		    theNbElem,
		    theIsElemNum,
		    theIsElemNames)
    {
      myTEntity = theTEntity;
      myTGeom = theTGeom;
      myTConn  = theTConn;
      myNbConn = theNbConn;
      myNbFacesIndex = theNbFacesIndex;
      myConn.resize(myNbConn);
      myFacesIndex.resize(myNbFacesIndex);
      myIndex.resize(theNbElem+1);
    }
    
    TTPolyedreInfo(const PMeshInfo& theMeshInfo, 
		   EEntiteMaillage theTEntity, 
		   EGeometrieElement theTGeom,
		   EConnectivite theTConn,
		   const TIntVector& theConnectivities,
		   const TIntVector& theFacesIndexes,
		   const TIntVector& theIndexes,
		   const TIntVector& theFamilyNums,
		   const TIntVector& theElemNums,
		   const TStringVector& theElemNames = TStringVector()):
      TElemInfoBase(theMeshInfo,
		    theFamilyNums,
		    theElemNums,
		    theElemNames)
    {
      myTEntity = theTEntity;
      myTGeom = theTGeom;
      myTConn  = theTConn;
      myNbConn = theConnectivities.size();
      myNbFacesIndex = theFacesIndexes.size();
      myConn = theConnectivities;
      myFacesIndex = theFacesIndexes;
      myIndex = theIndexes;
    }
  };

  //---------------------------------------------------------------
  template<TInt nPNOM, TInt nDESC, TInt nIDENT, TInt nNOM, TInt nLNOM, EVersion nV>
  struct TTCellInfo: 
    virtual TCellInfo, 
    virtual TTElemInfo<nPNOM,nDESC,nIDENT,nNOM,nLNOM,nV>
  {
    typedef TTElemInfo<nPNOM,nDESC,nIDENT,nNOM,nLNOM,nV> TElemInfoBase;

    TTCellInfo(const PMeshInfo& theMeshInfo, const PCellInfo& theInfo):
      TElemInfoBase(theMeshInfo,theInfo)
    {
      myTEntity = theInfo->GetEntity();
      myTGeom = theInfo->GetGeom();
      myTConn  = theInfo->GetConn();
      
      TInt aConnDim = GetNbNodes(myTGeom);
      myConn.resize(myNbElem*GetNbConn<nV>(myTGeom,myTEntity,myMeshInfo->myDim));
      for(TInt anElemId = 0; anElemId < myNbElem; anElemId++){
	for(TInt anConnId = 0; anConnId < aConnDim; anConnId++){
	  SetConn(anElemId,anConnId,theInfo->GetConn(anElemId,anConnId));
	}
      }
    }

    TTCellInfo(const PMeshInfo& theMeshInfo, 
	       TInt theNbElem,
	       EEntiteMaillage theTEntity, 
	       EGeometrieElement theTGeom,
	       EConnectivite theTConn = eNOD,
	       EBooleen theIsElemNum = eVRAI,
	       EBooleen theIsElemNames = eVRAI):
      TElemInfoBase(theMeshInfo,
		    theNbElem,
		    theIsElemNum,
		    theIsElemNames)
    {
      myTEntity = theTEntity;
      myTGeom = theTGeom;
      myTConn  = theTConn;
      myConn.resize(theNbElem*GetNbConn<nV>(theTGeom,myTEntity,theMeshInfo->myDim));
    }
    
    TTCellInfo(const PMeshInfo& theMeshInfo, 
	       EEntiteMaillage theTEntity, 
	       EGeometrieElement theTGeom,
	       EConnectivite theTConn,
	       const TIntVector& theConnectivities,
	       const TIntVector& theFamilyNums,
	       const TIntVector& theElemNums,
	       const TStringVector& theElemNames = TStringVector()):
      TElemInfoBase(theMeshInfo,
		    theFamilyNums,
		    theElemNums,
		    theElemNames)
    {
      myTEntity = theTEntity;
      myTGeom = theTGeom;
      myTConn  = theTConn;

      TInt aConnDim = GetNbNodes(myTGeom);
      myNbElem = theConnectivities.size() / aConnDim;
      myConn.resize(myNbElem*GetNbConn<nV>(myTGeom,myTEntity,myMeshInfo->myDim));
      for(TInt anElemId = 0; anElemId < myNbElem; anElemId++){
	for(TInt anConnId = 0; anConnId < aConnDim; anConnId++){
	  SetConn(anElemId,anConnId,theConnectivities[anElemId*aConnDim+anConnId]);
	}
      }

    }

    virtual TInt GetConnDim() const { 
      return GetNbConn<nV>(myTGeom,myTEntity,myMeshInfo->myDim);
    }

  };


  //---------------------------------------------------------------
  template<TInt nPNOM, TInt nDESC, TInt nIDENT, TInt nNOM, TInt nLNOM, EVersion nV>
  struct TTFieldInfo: 
    virtual TFieldInfo, 
    virtual TTNameInfo<nPNOM,nDESC,nIDENT,nNOM,nLNOM,nV>
  {
    typedef TTNameInfo<nPNOM,nDESC,nIDENT,nNOM,nLNOM,nV> TNameInfoBase;

    TTFieldInfo(const PMeshInfo& theMeshInfo, const PFieldInfo& theInfo):
      TNameInfoBase(theInfo->GetName())
    {
      myMeshInfo = theMeshInfo;

      myNbComp = theInfo->GetNbComp();
      myCompNames.resize(myNbComp*nPNOM+1);
      for(TInt anId = 0; anId < myNbComp; anId++){
	SetCompName(anId,theInfo->GetCompName(anId));
      }

      myUnitNames.resize(myNbComp*nPNOM+1);
      for(TInt anId = 0; anId < myNbComp; anId++){
	SetUnitName(anId,theInfo->GetUnitName(anId));
      }

      myType = theInfo->GetType();

      myIsLocal = theInfo->GetIsLocal();
      myNbRef = theInfo->GetNbRef();
    }

    TTFieldInfo(const PMeshInfo& theMeshInfo, 
		TInt theNbComp = 0,
		ETypeChamp theType = eFLOAT64,
		const std::string& theValue = "",
		EBooleen theIsLocal = eVRAI,
		TInt theNbRef = 1):
      TNameInfoBase(theValue)
    {
      myMeshInfo = theMeshInfo;

      myNbComp = theNbComp;
      myCompNames.resize(theNbComp*nPNOM+1);
      myUnitNames.resize(theNbComp*nPNOM+1);

      myType = theType;

      myIsLocal = theIsLocal;
      myNbRef = theNbRef;
    }
    
    virtual std::string GetCompName(TInt theId) const { 
      return GetString(theId,nPNOM,myCompNames);
    }

    virtual void SetCompName(TInt theId, const std::string& theValue){
      SetString(theId,nPNOM,myCompNames,theValue);
    }

    virtual std::string GetUnitName(TInt theId) const { 
      return GetString(theId,nPNOM,myUnitNames);
    }

    virtual void SetUnitName(TInt theId, const std::string& theValue){
      SetString(theId,nPNOM,myUnitNames,theValue);
    }
  };


  //---------------------------------------------------------------
  template<TInt nPNOM, TInt nDESC, TInt nIDENT, TInt nNOM, TInt nLNOM, EVersion nV>
  struct TTTimeStampInfo: virtual TTimeStampInfo
  {
    TTTimeStampInfo(const PFieldInfo& theFieldInfo, const PTimeStampInfo& theInfo)
    {
      myFieldInfo = theFieldInfo;

      myEntity = theInfo->GetEntity();
      myGeom = theInfo->GetGeom();

      myNbGauss = theInfo->GetNbGauss();
      myNumDt = theInfo->GetNumDt();
      myNumOrd = theInfo->GetNumOrd();
      myDt = theInfo->GetDt();

      myUnitDt.resize(nPNOM+1);
      SetUnitDt(theInfo->GetUnitDt());

      myGaussName.resize(nNOM+1);
      SetGaussName(theInfo->GetGaussName());
    }

    TTTimeStampInfo(const PFieldInfo& theFieldInfo, 
		    EEntiteMaillage theEntity,
		    const TGeom& theGeom,
		    TInt theNbGauss = 0,
		    TInt theNumDt = 0,
		    TInt theNumOrd = 0,
		    TFloat theDt = 0,
		    const std::string& theUnitDt = "",
		    const std::string& theGaussName = "")
    {
      myFieldInfo = theFieldInfo;

      myEntity = theEntity;
      myGeom = theGeom;

      myNbGauss = theNbGauss;
      myNumDt = theNumDt;
      myNumOrd = theNumDt;
      myDt = theDt;

      myUnitDt.resize(nPNOM+1);
      SetUnitDt(theUnitDt);

      myGaussName.resize(nNOM+1);
      SetGaussName(theGaussName);
    }

    virtual std::string GetGaussName() const { 
      return GetString(0,nNOM,myGaussName);
    }

    virtual void SetGaussName(const std::string& theValue){
      SetString(0,nNOM,myGaussName,theValue);
    }

    virtual std::string GetUnitDt() const { 
      return GetString(0,nPNOM,myUnitDt);
    }

    virtual void SetUnitDt(const std::string& theValue){
      SetString(0,nPNOM,myUnitDt,theValue);
    }
  };


  //---------------------------------------------------------------
  template<TInt nPNOM, TInt nDESC, TInt nIDENT, TInt nNOM, TInt nLNOM, EVersion nV>
  struct TTTimeStampVal: virtual TTimeStampVal
  {
    TTTimeStampVal(const PTimeStampInfo& theTimeStampInfo, const PTimeStampVal& theInfo)
    {
      myTimeStampInfo = theTimeStampInfo;

      myPflMode = theInfo->GetPflMode();

      myPflName.resize(nNOM+1);
      SetPflName(theInfo->GetPflName());

      TInt aNbComp = theTimeStampInfo->GetFieldInfo()->GetNbComp();
      const TGeom& aTGeom = theTimeStampInfo->GetGeom();
      TInt aNbGauss = theTimeStampInfo->GetNbGauss();
      TGeom::const_iterator anIter = aTGeom.begin();
      for(; anIter != aTGeom.end(); anIter++){
	const EGeometrieElement& aGeom = anIter->first;
	TInt aNbElem = anIter->second;
	TInt aSize = aNbElem*aNbComp*aNbGauss;
	TValue& aValue = myMeshValue[aGeom];
	aValue.resize(aSize);
	for(TInt anElemId = 0; anElemId < aNbElem; anElemId++){
	  for(TInt aCompId = 0; aCompId < aNbComp; aCompId++){
	    for(TInt aGaussId = 0; aGaussId < aNbGauss; aGaussId++){
	      SetVal(aGeom,anElemId,aCompId,theInfo->GetVal(aGeom,anElemId,aCompId,aGaussId),aGaussId);
	    }
	  }
	}
      }
    }

    TTTimeStampVal(const PTimeStampInfo& theTimeStampInfo,
		   const std::string& thePflName = "",
		   EModeProfil thePflMode = eCOMPACT)
    {
      myTimeStampInfo = theTimeStampInfo;

      myPflMode = thePflMode;

      myPflName.resize(nNOM+1);
      SetPflName(thePflName);

      TInt aNbComp = theTimeStampInfo->myFieldInfo->myNbComp;
      TInt aNbGauss = theTimeStampInfo->myNbGauss;
      const TGeom& aTGeom = theTimeStampInfo->myGeom;
      TGeom::const_iterator anIter = aTGeom.begin();
      for(; anIter != aTGeom.end(); anIter++){
	const EGeometrieElement& aGeom = anIter->first;
	TInt aNb = anIter->second*aNbComp*aNbGauss;
	myMeshValue[aGeom].resize(aNb);
      }
    }

    virtual std::string GetPflName() const { 
      return GetString(0,nNOM,myPflName);
    }
    
    virtual void SetPflName(const std::string& theValue){
      SetString(0,nNOM,myPflName,theValue);
    }
  };

}

#endif
