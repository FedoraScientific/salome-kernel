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
//  File   : SALOMEDSImpl_StudyManager.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDSImpl_StudyManager.hxx"

#include "DF_ChildIterator.hxx"
#include "HDFexplorer.hxx"

#include "SALOMEDSImpl_Attributes.hxx"
#include "SALOMEDSImpl_Tool.hxx"
#include "SALOMEDSImpl_SComponent.hxx"
#include "SALOMEDSImpl_GenericAttribute.hxx"
#include <map>

#include "HDFOI.hxx"
#include <iostream>
#include <stdlib.h>

using namespace std;

#define USE_CASE_LABEL_ID                       "0:2"

static void SaveAttributes(const SALOMEDSImpl_SObject& SO, HDFgroup *hdf_group_sobject);
static void ReadAttributes(SALOMEDSImpl_Study*, const SALOMEDSImpl_SObject&, HDFdataset* );
static void BuildTree (SALOMEDSImpl_Study*, HDFgroup*);
static void Translate_IOR_to_persistentID (const SALOMEDSImpl_SObject&,
					   SALOMEDSImpl_Driver*, bool isMultiFile, bool isASCII);

//============================================================================
/*! Function : SALOMEDSImpl_StudyManager
 *  Purpose  : SALOMEDSImpl_StudyManager constructor
 */
//============================================================================
SALOMEDSImpl_StudyManager::SALOMEDSImpl_StudyManager()
{
  _errorCode = "";
  _appli = new DF_Application();
  _IDcounter = 0;
  _clipboard = _appli->NewDocument("SALOME_STUDY");
}

//============================================================================
/*! Function : ~SALOMEDSImpl_StudyManager
 *  Purpose  : SALOMEDSImpl_StudyManager destructor
 */
//============================================================================
SALOMEDSImpl_StudyManager::~SALOMEDSImpl_StudyManager()
{
  // Destroy application
  delete _appli;    
}


//============================================================================
/*! Function : NewStudy
 *  Purpose  : Create a New Study of name study_name
 */
//==================================================T==========================
SALOMEDSImpl_Study* SALOMEDSImpl_StudyManager::NewStudy(const string& study_name)
{
  _errorCode = "";

  DF_Document* Doc = _appli->NewDocument("SALOME_STUDY");

  SALOMEDSImpl_Study* Study = new SALOMEDSImpl_Study(Doc, study_name);

  _IDcounter++;
  Study->StudyId( _IDcounter );

  // set Study properties
  SALOMEDSImpl_AttributeStudyProperties* aProp = Study->GetProperties();
  
  int month=0,day=0,year=0,hh=0,mn=0,ss=0;
  SALOMEDSImpl_Tool::GetSystemDate(year, month, day, hh, mn, ss);
  aProp->SetModification(SALOMEDSImpl_Tool::GetUserName(),
                         mn, hh, day, month, year);
  aProp->SetCreationMode(1);  //"from scratch"

  return Study;
}

//============================================================================
/*! Function : Open
 *  Purpose  : Open a Study from it's persistent reference
 */
//============================================================================
SALOMEDSImpl_Study* SALOMEDSImpl_StudyManager::Open(const string& aUrl)
{
  _errorCode = "";

  // open the HDFFile
  HDFfile *hdf_file =0;
  HDFgroup *hdf_group_study_structure =0;

  char* aC_HDFUrl;
  string aHDFUrl;
  bool isASCII = false;
  if (HDFascii::isASCII(aUrl.c_str())) {
    isASCII = true;
    char* aResultPath = HDFascii::ConvertFromASCIIToHDF(aUrl.c_str());
    aC_HDFUrl = new char[strlen(aResultPath) + 19];
    sprintf(aC_HDFUrl, "%shdf_from_ascii.hdf", aResultPath);
    delete(aResultPath);
    aHDFUrl = aC_HDFUrl;
    delete aC_HDFUrl;
  } else {
    aHDFUrl = aUrl;
  }

  
  hdf_file = new HDFfile((char*)aHDFUrl.c_str());
  try {
    hdf_file->OpenOnDisk(HDF_RDONLY);// mpv: was RDWR, but opened file can be write-protected too
  }
  catch (HDFexception)
    {
//#ifndef WNT
//      char eStr[strlen(aUrl.ToCString())+17];
//#else
	  char *eStr;
	  eStr = new char[strlen(aUrl.c_str())+17];
//#endif
      sprintf(eStr,"Can't open file %s",aUrl.c_str());
//#ifdef WNT
	  delete [] eStr;
//#endif
      _errorCode = string(eStr);
      return NULL;
    }

  // Temporary aStudyUrl in place of study name
  DF_Document* Doc = _appli->NewDocument("SALOME_STUDY");

  SALOMEDSImpl_Study* Study = new SALOMEDSImpl_Study(Doc, aUrl);

  _IDcounter++;
  Study->StudyId( _IDcounter );

  // Assign the value of the URL in the study object
  Study->URL (aUrl);

  SALOMEDSImpl_AttributePersistentRef::Set(Doc->Main(), aUrl);

  if (!hdf_file->ExistInternalObject("STUDY_STRUCTURE")) {
     _errorCode = "Study is empty";
    return Study;
  }

  //Create  the Structure of the Document
  hdf_group_study_structure = new HDFgroup("STUDY_STRUCTURE",hdf_file);

  try {
    BuildTree (Study, hdf_group_study_structure);
  }
  catch (HDFexception)
    {
//#ifndef WNT
//      char eStr[strlen(aUrl.ToCString())+17];
//#else
	  char *eStr = new char [strlen(aUrl.c_str())+17];
//#endif
      sprintf(eStr,"Can't open file %s", aUrl.c_str());
      _errorCode = string(eStr);
      return NULL;
    }

  hdf_file->CloseOnDisk();

  if (isASCII) {
    vector<string> aFilesToRemove;
    aFilesToRemove.push_back("hdf_from_ascii.hdf");
    SALOMEDSImpl_Tool::RemoveTemporaryFiles(SALOMEDSImpl_Tool::GetDirFromPath(aHDFUrl), aFilesToRemove, true);
  }

  delete hdf_file; // all related hdf objects will be deleted

  return Study;
}



//============================================================================
/*! Function : Close
 *  Purpose  : Close a study.
 *             If the study hasn't been saved, ask the user to confirm the
 *             close action without saving
 */

//============================================================================
void  SALOMEDSImpl_StudyManager::Close(SALOMEDSImpl_Study* aStudy)
{
  _errorCode = "";

  if(!aStudy) {
    _errorCode = "Study is null";
    return;
  }

  aStudy->Close();
}

//============================================================================
/*! Function : Save
 *  Purpose  : Save a Study to it's persistent reference
 */
//============================================================================
bool SALOMEDSImpl_StudyManager::Save(SALOMEDSImpl_Study* aStudy,
				     SALOMEDSImpl_DriverFactory* aFactory,
				     bool theMultiFile)
{
  _errorCode = "";

  string url = aStudy->URL();
  if (url.empty()) {
    _errorCode = "No path specified to save the study. Nothing done";
    return false;
  }
  else {
    return Impl_SaveAs(url,aStudy, aFactory, theMultiFile, false);
  }

  return false;
}

bool SALOMEDSImpl_StudyManager::SaveASCII(SALOMEDSImpl_Study* aStudy,
					  SALOMEDSImpl_DriverFactory* aFactory,
					  bool theMultiFile)
{
  _errorCode = "";

  string url = aStudy->URL();
  if (url.empty()) {
    _errorCode = "No path specified to save the study. Nothing done";
    return false;
  }
  else {
    return Impl_SaveAs(url,aStudy, aFactory, theMultiFile, true);
  }

  return false;
}

//=============================================================================
/*! Function : SaveAs
 *  Purpose  : Save a study to the persistent reference aUrl
 */
//============================================================================
bool SALOMEDSImpl_StudyManager::SaveAs(const string& aUrl,
				       SALOMEDSImpl_Study* aStudy,
				       SALOMEDSImpl_DriverFactory* aFactory,
				       bool theMultiFile)
{
  _errorCode = "";
  return Impl_SaveAs(aUrl,aStudy, aFactory, theMultiFile, false);
}

bool SALOMEDSImpl_StudyManager::SaveAsASCII(const string& aUrl,
					    SALOMEDSImpl_Study* aStudy,
					    SALOMEDSImpl_DriverFactory* aFactory,
					    bool theMultiFile)
{
  _errorCode = "";
  return Impl_SaveAs(aUrl,aStudy, aFactory, theMultiFile, true);
}

//============================================================================
/*! Function : GetOpenStudies
 *  Purpose  : Get name list of open studies in the session
 */
//============================================================================
vector<SALOMEDSImpl_Study*> SALOMEDSImpl_StudyManager::GetOpenStudies()
{
  _errorCode = "";
  vector<SALOMEDSImpl_Study*> aList;

  int nbDocs = _appli->NbDocuments();

  if(nbDocs == 0) {
    _errorCode = "No active study in this session";
    return aList;
  }
  else {
    SALOMEDSImpl_Study* aStudy;
    vector<int> ids = _appli->GetDocumentIDs();
    for (int i = 0, len = ids.size(); i<len; i++) {
      DF_Document* D = _appli->GetDocument(ids[i]);
      if(D == _clipboard) continue;
      aStudy = SALOMEDSImpl_Study::GetStudy(D->Main());
      if(!aStudy) continue;
      aList.push_back(aStudy);
    }
  }

  return aList;
}

//============================================================================
/*! Function : GetStudyByName
 *  Purpose  : Get a study from its name
 */
//============================================================================
SALOMEDSImpl_Study* SALOMEDSImpl_StudyManager::GetStudyByName
                                   (const string& aStudyName)
{
  _errorCode = "";
  int nbDocs = _appli->NbDocuments();

  if (nbDocs == 0) {
    _errorCode = "No active study in this session";
    return NULL;
  }
  else {
    vector<SALOMEDSImpl_Study*> studies = GetOpenStudies();
    for (int i = 0, len = studies.size(); i<len; i++) {
      if (studies[i]->Name() == aStudyName) return studies[i];
    }
  }

  _errorCode = string("Found no study with the name ") + aStudyName;
  return NULL;
}

//============================================================================
/*! Function : GetStudyByID
 *  Purpose  : Get a study from its ID
 */
//============================================================================
SALOMEDSImpl_Study* SALOMEDSImpl_StudyManager::GetStudyByID(int aStudyID)
{
  _errorCode = "";
  int nbDocs = _appli->NbDocuments();

  if (nbDocs == 0) {
    _errorCode = "No active study in this session";
    return NULL;
  }
  else {
    vector<SALOMEDSImpl_Study*> studies = GetOpenStudies();
    for (int i = 0, len = studies.size(); i<len; i++) {
      if (studies[i]->StudyId() == aStudyID) return studies[i];
    }
  }

  _errorCode = "Found no study with the given ID";
  return NULL;
}

//=============================================================================
/*! Function : _SaveProperties
 *  Purpose  : save the study properties in HDF file
 */
//============================================================================
bool SALOMEDSImpl_StudyManager::Impl_SaveProperties(SALOMEDSImpl_Study* aStudy,
                                                    HDFgroup *hdf_group)
{
  _errorCode = "";

  HDFdataset *hdf_dataset = 0;
  hdf_size size[1];
  hdf_int32 name_len;

  // add modifications list (user and date of save)
  SALOMEDSImpl_AttributeStudyProperties* aProp = aStudy->GetProperties();
  int aLocked = aProp->IsLocked();
  if (aLocked) aProp->SetLocked(false);

  int month=0,day=0,year=0,hh=0,mn=0,ss=0;
  SALOMEDSImpl_Tool::GetSystemDate(year, month, day, hh, mn, ss);
  aProp->SetModification(SALOMEDSImpl_Tool::GetUserName(),
                         mn, hh, day, month, year);

  if (aLocked) aProp->SetLocked(true);

  vector<string> aNames;
  vector<int> aMinutes, aHours, aDays, aMonths, aYears;

  aProp->GetModifications(aNames, aMinutes, aHours, aDays, aMonths, aYears);

  int aLength = 0, anIndex, i;
  for(i=1; i<=aNames.size(); i++)
    aLength += aNames[i-1].size() + 1;

  //string length: 1 byte = locked flag, 1 byte = modified flag, (12 + name length + 1) for each name and date, "zero" byte
  char* aProperty = new char[3 + aLength + 12 * aNames.size()];


  sprintf(aProperty,"%c%c", (char)aProp->GetCreationMode(),  (aProp->IsLocked())?'l':'u');

  aLength = aNames.size();
  int a = 2;
  for(anIndex = 0; anIndex<aLength; anIndex++) {
    sprintf(&(aProperty[a]),"%2d%2d%2d%2d%4d%s",
	    (int)(aMinutes[anIndex]),
	    (int)(aHours[anIndex]),
	    (int)(aDays[anIndex]),
	    (int)(aMonths[anIndex]),
	    (int)(aYears[anIndex]),
	    aNames[anIndex].c_str());
    a = strlen(aProperty);
    aProperty[a++] = 1;
  }
  aProperty[a] = 0;

  name_len = (hdf_int32) a;
  size[0] = name_len + 1 ;
  hdf_dataset = new HDFdataset("AttributeStudyProperties",hdf_group,HDF_STRING,size,1);
  hdf_dataset->CreateOnDisk();
  hdf_dataset->WriteOnDisk(aProperty);
  hdf_dataset->CloseOnDisk();
  hdf_dataset=0; //will be deleted by hdf_sco_group destructor
  delete [] aProperty;

  aProp->SetModified(0);
  return true;
}

//=============================================================================
/*! Function : _SaveAs
 *  Purpose  : save the study in HDF file
 */
//============================================================================
bool SALOMEDSImpl_StudyManager::Impl_SaveAs(const string& aUrl,
					    SALOMEDSImpl_Study* aStudy,
					    SALOMEDSImpl_DriverFactory* aFactory,
					    bool theMultiFile,
					    bool theASCII)
{
  // HDF File will be composed of differents part :
  // * For each ComponentDataType, all data created by the component
  //   Informations in data group hdf_group_datacomponent
  // * Study Structure -> Exactly what is contained in Document
  //   Informations in data group hdf_group_study_structure

  _errorCode = "";

  HDFfile *hdf_file=0;
  HDFgroup *hdf_group_study_structure =0;
  HDFgroup *hdf_sco_group =0;
  HDFgroup *hdf_sco_group2 =0;

  HDFgroup *hdf_group_datacomponent =0;
  HDFdataset *hdf_dataset =0;
  hdf_size size[1];
  hdf_int32 name_len = 0;
  char *component_name = 0;

  if(!aStudy) {
    _errorCode = "Study is null";
    return false;
  }

  int aLocked = aStudy->GetProperties()->IsLocked();
  if (aLocked) aStudy->GetProperties()->SetLocked(false);

  SALOMEDSImpl_StudyBuilder* SB= aStudy->NewBuilder();
  map<string, SALOMEDSImpl_Driver*> aMapTypeDriver;

  try
    {
      // mpv 15.12.2003: for saving components we have to load all data from all modules
      SALOMEDSImpl_SComponentIterator itcomponent1 = aStudy->NewComponentIterator();
      for (; itcomponent1.More(); itcomponent1.Next())
	{
	  SALOMEDSImpl_SComponent sco = itcomponent1.Value();
	  // if there is an associated Engine call its method for saving
	  string IOREngine;
	  try {
	    if (!sco.ComponentIOR(IOREngine)) {
	      string aCompType = sco.GetComment();
	      if (!aCompType.empty()) {

		SALOMEDSImpl_Driver* aDriver = aFactory->GetDriverByType(aCompType);
		aMapTypeDriver[aCompType] = aDriver;

		if (aDriver != NULL) {
		  if(!SB->LoadWith(sco, aDriver)) {
		    _errorCode = SB->GetErrorCode();
		    return false;
		  }
		}
	      }
	    }
	  } catch(...) {
	    _errorCode = "Can not restore information to resave it";
	    return false;
	  }
	}

      string anOldName = aStudy->Name();
      aStudy->URL(aUrl);

      // To change for Save
      // Do not have to do a new file but just a Open??? Rewrite all informations after erasing evrything??
      hdf_file = new HDFfile((char*)aUrl.c_str());
      hdf_file->CreateOnDisk();

      //-----------------------------------------------------------------------
      // 1 - Create a groupe for each SComponent and Update the PersistanceRef
      //-----------------------------------------------------------------------
      hdf_group_datacomponent = new HDFgroup("DATACOMPONENT",hdf_file);
      hdf_group_datacomponent->CreateOnDisk();

      SALOMEDSImpl_SComponentIterator itcomponent = aStudy->NewComponentIterator();

      for (; itcomponent.More(); itcomponent.Next())
    	{
	  SALOMEDSImpl_SComponent sco = itcomponent.Value();

	  string scoid = sco.GetID();
	  hdf_sco_group = new HDFgroup((char*)scoid.c_str(), hdf_group_datacomponent);
	  hdf_sco_group->CreateOnDisk();

	  string componentDataType = sco.ComponentDataType();
	  string IOREngine;
	  if (sco.ComponentIOR(IOREngine))
	    {
	      SALOMEDSImpl_Driver* Engine = NULL;
	      if(aMapTypeDriver.find(componentDataType) != aMapTypeDriver.end()) {
		// we have found the associated engine to write the data
		Engine = aMapTypeDriver[componentDataType];
	      }
	      else {
		Engine = aFactory->GetDriverByIOR(IOREngine);
	      }

	      if (Engine != NULL)
		{
		  SALOMEDSImpl_TMPFile* aStream = NULL;
		  long length = 0;

                  if (theASCII) aStream = Engine->SaveASCII(sco,
							    SALOMEDSImpl_Tool::GetDirFromPath(aUrl),
							    length,
							    theMultiFile);
		  else aStream = Engine->Save(sco,
					      SALOMEDSImpl_Tool::GetDirFromPath(aUrl),
					      length,
					      theMultiFile);
		  HDFdataset *hdf_dataset;
		  hdf_size aHDFSize[1]; 
		  if(length > 0) {  //The component saved some auxiliary files, then put them into HDF file

		    aHDFSize[0] = length;

		    HDFdataset *hdf_dataset = new HDFdataset("FILE_STREAM", hdf_sco_group, HDF_STRING, aHDFSize, 1);
		    hdf_dataset->CreateOnDisk();
		    hdf_dataset->WriteOnDisk(aStream->Data());  //Save the stream in the HDF file
		    hdf_dataset->CloseOnDisk();
		  }

		  if(aStream) delete aStream;

		  // store multifile state
		  aHDFSize[0] = 2;
		  hdf_dataset = new HDFdataset("MULTIFILE_STATE", hdf_sco_group, HDF_STRING, aHDFSize, 1);
		  hdf_dataset->CreateOnDisk();
		  hdf_dataset->WriteOnDisk((void*)(theMultiFile?"M":"S")); // save: multi or single
		  hdf_dataset->CloseOnDisk();
		  hdf_dataset=0; //will be deleted by hdf_sco_AuxFiles destructor
		  // store ASCII state
		  aHDFSize[0] = 2;
		  hdf_dataset = new HDFdataset("ASCII_STATE", hdf_sco_group, HDF_STRING, aHDFSize, 1);
		  hdf_dataset->CreateOnDisk();
		  hdf_dataset->WriteOnDisk((void*)(theASCII?"A":"B")); // save: ASCII or BINARY
		  hdf_dataset->CloseOnDisk();
		  hdf_dataset=0; //will be deleted by hdf_sco_AuxFiles destructor
		  // Creation of the persistance reference  attribute
		  Translate_IOR_to_persistentID (sco, Engine, theMultiFile, theASCII);
		}
	    }
	  hdf_sco_group->CloseOnDisk();
	  hdf_sco_group=0; // will be deleted by hdf_group_datacomponent destructor
	}
      hdf_group_datacomponent->CloseOnDisk();
      hdf_group_datacomponent =0;  // will be deleted by hdf_file destructor

      //-----------------------------------------------------------------------
      //3 - Write the Study Structure
      //-----------------------------------------------------------------------
      hdf_group_study_structure = new HDFgroup("STUDY_STRUCTURE",hdf_file);
      hdf_group_study_structure->CreateOnDisk();
      // save component attributes
      SALOMEDSImpl_SComponentIterator itcomp = aStudy->NewComponentIterator();
      for (; itcomp.More(); itcomp.Next())
	{
	  SALOMEDSImpl_SComponent SC = itcomp.Value();
	  string scid = SC.GetID();
	  hdf_sco_group2 = new HDFgroup((char*)scid.c_str(), hdf_group_study_structure);
	  hdf_sco_group2->CreateOnDisk();
          SaveAttributes(SC, hdf_sco_group2);
	  // ComponentDataType treatment
	  component_name = (char*)SC.ComponentDataType().c_str();
	  name_len = (hdf_int32)strlen(component_name);
	  size[0] = name_len +1 ;
	  hdf_dataset = new HDFdataset("COMPONENTDATATYPE",hdf_sco_group2,HDF_STRING,size,1);
	  hdf_dataset->CreateOnDisk();
	  hdf_dataset->WriteOnDisk(component_name);
	  hdf_dataset->CloseOnDisk();
	  hdf_dataset=0; //will be deleted by hdf_sco_group destructor
	  Impl_SaveObject(SC, hdf_sco_group2);
	  hdf_sco_group2->CloseOnDisk();
 	  hdf_sco_group2=0; // will be deleted by hdf_group_study_structure destructor
	}
      //-----------------------------------------------------------------------
      //4 - Write the Study UseCases Structure
      //-----------------------------------------------------------------------
      SALOMEDSImpl_SObject aSO = aStudy->FindObjectID(USE_CASE_LABEL_ID);
      if (aSO) {
	HDFgroup *hdf_soo_group = new HDFgroup(USE_CASE_LABEL_ID,hdf_group_study_structure);
	hdf_soo_group->CreateOnDisk();
	SaveAttributes(aSO, hdf_soo_group);
	Impl_SaveObject(aSO, hdf_soo_group);
	hdf_soo_group->CloseOnDisk();
	hdf_soo_group=0; // will be deleted by hdf_group_study_structure destructor
      }

      if (aLocked) aStudy->GetProperties()->SetLocked(true);
      //-----------------------------------------------------------------------
      //5 - Write the Study Properties
      //-----------------------------------------------------------------------
      name_len = (hdf_int32) aStudy->Name().size();
      size[0] = name_len +1 ;
      hdf_dataset = new HDFdataset("STUDY_NAME",hdf_group_study_structure,HDF_STRING,size,1);
      hdf_dataset->CreateOnDisk();
      char* studid = (char*)aStudy->Name().c_str();
      hdf_dataset->WriteOnDisk(studid);
      hdf_dataset->CloseOnDisk();
      hdf_dataset=0; // will be deleted by hdf_group_study_structure destructor

      Impl_SaveProperties(aStudy, hdf_group_study_structure);
      hdf_group_study_structure->CloseOnDisk();
      hdf_file->CloseOnDisk();

      aStudy->IsSaved(true);
      hdf_group_study_structure =0; // will be deleted by hdf_file destructor
      delete hdf_file; // recursively deletes all hdf objects...
    }
  catch (HDFexception)
    {
      _errorCode = "HDFexception ! ";
      return false;
    }
  catch(std::exception& exc)
    {
      _errorCode = const_cast<char*>(exc.what());
      return false;
    }
  catch(...)
    {
      _errorCode = "Unknown exception ! ";
      return false;
    }
  if (theASCII) { // save file in ASCII format
    HDFascii::ConvertFromHDFToASCII(aUrl.c_str(), true);
  }

  return true;
}

//============================================================================
/*! Function : Impl_SaveObject
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_StudyManager::Impl_SaveObject(const SALOMEDSImpl_SObject& SC,
						HDFgroup *hdf_group_datatype)
{
  _errorCode = "";

  // Write in group hdf_group_datatype all informations of SObject SC
  // Iterative function to parse all SObjects under a SComponent

  HDFgroup *hdf_group_sobject = 0;

  DF_ChildIterator itchild(SC.GetLabel());
  for (; itchild.More(); itchild.Next())
    {

      // mpv: don't save empty labels
      vector<DF_Attribute*> attr = itchild.Value().GetAttributes();
      if (attr.size() == 0) {  //No attributes on the label
	DF_ChildIterator subchild(itchild.Value());
	if (!subchild.More()) {
	  continue;
	}
	subchild.Init(itchild.Value(), true);
	bool anEmpty = true;
	for (; subchild.More() && anEmpty; subchild.Next()) {
	  vector<DF_Attribute*> attr2 = subchild.Value().GetAttributes();
	  if (attr2.size()) {
	    anEmpty = false;  //There are attributes on the child label
	    break;
	  }
	}
	if (anEmpty) continue;
      }

      SALOMEDSImpl_SObject SO = SALOMEDSImpl_Study::SObject(itchild.Value());

      char* scoid = (char*) SO.GetID().c_str();
      hdf_group_sobject = new HDFgroup(scoid, hdf_group_datatype);
      hdf_group_sobject->CreateOnDisk();
      SaveAttributes(SO, hdf_group_sobject);
      Impl_SaveObject(SO, hdf_group_sobject);
      hdf_group_sobject->CloseOnDisk();
      hdf_group_sobject =0; // will be deleted by father hdf object destructor
    }

  return true;
}

//============================================================================
/*! Function : Impl_SubstituteSlash
 *  Purpose  :
 */
//============================================================================
string SALOMEDSImpl_StudyManager::Impl_SubstituteSlash(const string& aUrl)
{
  _errorCode = "";

  std::string theUrl(aUrl);
  for(int i = 0; i<theUrl.size(); i++)
    if(theUrl[i] == '/') theUrl[i] = ':';
  return theUrl;
}

//============================================================================
/*! Function : GetDocumentOfStudy
 *  Purpose  :
 */
//============================================================================
DF_Document* SALOMEDSImpl_StudyManager::GetDocumentOfStudy(SALOMEDSImpl_Study* theStudy)
{
  _errorCode = "";
  return theStudy->_doc;
}

//============================================================================
/*! Function : CanCopy
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_StudyManager::CanCopy(const SALOMEDSImpl_SObject& theObject,
					SALOMEDSImpl_Driver* theEngine)
{
  _errorCode = "";
  SALOMEDSImpl_SComponent aComponent = theObject.GetFatherComponent();
  if (!aComponent) return false;
  if (aComponent.GetLabel() == theObject.GetLabel()) return false;
  string IOREngine;
  if (!aComponent.ComponentIOR(IOREngine)) return false;
  if (theEngine == NULL) return false;
  return theEngine->CanCopy(theObject);
}

//============================================================================
/*! Function : CopyLabel
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_StudyManager::CopyLabel(SALOMEDSImpl_Study* theSourceStudy,
					  SALOMEDSImpl_Driver* theEngine,
					  const int theSourceStartDepth,
					  const DF_Label& theSource,
					  const DF_Label& theDestinationMain)
{
  _errorCode = "";

  int a;
  DF_Label aTargetLabel = theDestinationMain;
  DF_Label aAuxTargetLabel = theDestinationMain.Father().FindChild(2);
  for(a = theSource.Depth() - theSourceStartDepth; a > 0 ; a--) {
    DF_Label aSourceLabel = theSource;
    for(int aNbFather = 1; aNbFather < a; aNbFather++) aSourceLabel = aSourceLabel.Father();
    aTargetLabel = aTargetLabel.FindChild(aSourceLabel.Tag());
    aAuxTargetLabel = aAuxTargetLabel.FindChild(aSourceLabel.Tag());
  }
  // iterate attributes
  vector<DF_Attribute*> attrList = theSource.GetAttributes();
  for(int i = 0, len = attrList.size(); i<len; i++) {
    DF_Attribute* anAttr = attrList[i];
    string type = SALOMEDSImpl_GenericAttribute::Impl_GetType(anAttr);
    if (type.substr(0, 17) == string("AttributeTreeNode")) continue; // never copy tree node attribute
    if (type == string("AttributeTarget")) continue; // and target attribute

    if (type == string("AttributeReference")) { // reference copied as Comment in aux tree
      DF_Label aReferenced = dynamic_cast<SALOMEDSImpl_AttributeReference*>(anAttr)->Get();
      string anEntry = aReferenced.Entry();
      // store the value of name attribute of referenced label
      SALOMEDSImpl_AttributeName* aNameAttribute;
      if ((aNameAttribute=(SALOMEDSImpl_AttributeName*)aReferenced.FindAttribute(SALOMEDSImpl_AttributeName::GetID()))) {
	anEntry += " ";
	anEntry += aNameAttribute->Value();
      }
      SALOMEDSImpl_AttributeComment::Set(aAuxTargetLabel, anEntry);
      continue;
    }

    if (type == string("AttributeIOR")) { // IOR => ID and TMPFile of Engine
      string anEntry = theSource.Entry();
      SALOMEDSImpl_SObject aSO = theSourceStudy->FindObjectID(anEntry);
      int anObjID;
      long aLen;
      SALOMEDSImpl_TMPFile* aStream = theEngine->CopyFrom(aSO, anObjID, aLen);
      string aResStr("");
      for(a = 0; a < aLen; a++) {
	aResStr += (char)(aStream->Get(a));
      }

      if(aStream) delete aStream;

      SALOMEDSImpl_AttributeInteger::Set(aAuxTargetLabel, anObjID);
      SALOMEDSImpl_AttributeName::Set(aAuxTargetLabel, aResStr);
      continue;
    }
    DF_Attribute* aNewAttribute = anAttr->NewEmpty();
    aTargetLabel.AddAttribute(aNewAttribute);
    anAttr->Paste(aNewAttribute);
  }

  return true;
}

//============================================================================
/*! Function : Copy
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_StudyManager::Copy(const SALOMEDSImpl_SObject& theObject,
				     SALOMEDSImpl_Driver* theEngine)
{
  _errorCode = "";

  // adoptation for alliances datamodel copy: without IOR attributes !!!
  bool aStructureOnly; // copy only SObjects and attributes without component help
  aStructureOnly = !theObject.GetLabel().IsAttribute(SALOMEDSImpl_AttributeIOR::GetID());

  // get component-engine
  SALOMEDSImpl_Study* aStudy = theObject.GetStudy();

  // CAF document of current study usage
  DF_Document* aDocument = GetDocumentOfStudy(aStudy);
  if (!aDocument) {
    _errorCode = "Document is null";
    return false;
  }

  //Clear the clipboard
  _clipboard->Main().Root().ForgetAllAttributes(true);
  _appli->Close(_clipboard);
  _clipboard = _appli->NewDocument("SALOME_STUDY");

  // set component data type to the name attribute of root label
  if (!aStructureOnly) {
    SALOMEDSImpl_AttributeComment::Set(_clipboard->Main().Root(),
				       theEngine->ComponentDataType());
  }
  // set to the Root label integer attribute: study id
  SALOMEDSImpl_AttributeInteger::Set(_clipboard->Main().Root(), aStudy->StudyId());
  // iterate all theObject's label children
  DF_Label aStartLabel = theObject.GetLabel();
  int aSourceStartDepth = aStartLabel.Depth();

  // copy main source label
  CopyLabel(aStudy, theEngine, aSourceStartDepth, aStartLabel, _clipboard->Main());

  // copy all subchildren of the main source label (all levels)
  DF_ChildIterator anIterator(aStartLabel, true);
  for(; anIterator.More(); anIterator.Next()) {
    CopyLabel(aStudy, theEngine, aSourceStartDepth, anIterator.Value(), _clipboard->Main());
  }

  return true;
}
//============================================================================
/*! Function : CanPaste
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_StudyManager::CanPaste(const SALOMEDSImpl_SObject& theObject,
					 SALOMEDSImpl_Driver* theEngine)
{
  _errorCode = "";

  if (!_clipboard) {
    _errorCode = "Clipboard is null";
    return false;
  }

  SALOMEDSImpl_AttributeComment* aCompName = NULL;
  if (!(aCompName=(SALOMEDSImpl_AttributeComment*)_clipboard->Main().Root().FindAttribute(SALOMEDSImpl_AttributeComment::GetID()))) {
    _errorCode = "Clipboard has no component type";
    return false;
  }
  SALOMEDSImpl_AttributeInteger* anObjID;
  if (!(anObjID=(SALOMEDSImpl_AttributeInteger*)_clipboard->Main().Father().FindChild(2).FindAttribute(SALOMEDSImpl_AttributeInteger::GetID()))) {
    _errorCode = "Clipboard has no object id";
    return false;
  }
  SALOMEDSImpl_SComponent aComponent = theObject.GetFatherComponent();
  if (!aComponent) {
    _errorCode = "Object doesn't belong to component";
    return false;
  }

  string IOREngine;
  if (!aComponent.ComponentIOR(IOREngine)) {
    _errorCode = "component has no IOR";
    return false;
  }
  return theEngine->CanPaste(aCompName->Value(), anObjID->Value());
}

//============================================================================
/*! Function : PasteLabel
 *  Purpose  :
 */
//============================================================================
DF_Label SALOMEDSImpl_StudyManager::PasteLabel(SALOMEDSImpl_Study* theDestinationStudy,
					       SALOMEDSImpl_Driver* theEngine,
					       const DF_Label& theSource,
					       const DF_Label& theDestinationStart,
					       const int theCopiedStudyID,
					       const bool isFirstElement)
{
  _errorCode = "";

  // get corresponding source, target and auxiliary labels
  DF_Label aTargetLabel = theDestinationStart;

  DF_Label aAuxSourceLabel = theSource.Root().FindChild(2);
  int a;
  if (!isFirstElement) {
    for(a = theSource.Depth() - 1; a > 0 ; a--) {
      DF_Label aSourceLabel = theSource;
      for(int aNbFather = 1; aNbFather < a; aNbFather++) aSourceLabel = aSourceLabel.Father();
      aTargetLabel = aTargetLabel.FindChild(aSourceLabel.Tag());
      aAuxSourceLabel = aAuxSourceLabel.FindChild(aSourceLabel.Tag());
    }
  }

  // check auxiliary label for TMPFile => IOR
  SALOMEDSImpl_AttributeName* aNameAttribute = NULL;
  if ((aNameAttribute=(SALOMEDSImpl_AttributeName*)aAuxSourceLabel.FindAttribute(SALOMEDSImpl_AttributeName::GetID()))) {
    SALOMEDSImpl_AttributeInteger* anObjID = (SALOMEDSImpl_AttributeInteger*)aAuxSourceLabel.FindAttribute(SALOMEDSImpl_AttributeInteger::GetID());
    SALOMEDSImpl_AttributeComment* aComponentName = (SALOMEDSImpl_AttributeComment*)theSource.Root().FindAttribute(SALOMEDSImpl_AttributeComment::GetID());
    string aCompName = aComponentName->Value();

    if (theEngine->CanPaste(aCompName, anObjID->Value())) {
      std::string aTMPStr = aNameAttribute->Value();
      int aLen = aTMPStr.size();
      unsigned char* aStream = NULL;
      if(aLen > 0) {
	aStream = new unsigned char[aLen+10];
	for(a = 0; a < aLen; a++) {
	  aStream[a] = aTMPStr[a];
	}
      }

      string anEntry = aTargetLabel.Entry();
      SALOMEDSImpl_SObject aPastedSO = theDestinationStudy->FindObjectID(anEntry);

      if (isFirstElement) {
	string aDestEntry = theEngine->PasteInto(aStream,
						 aLen,
						 anObjID->Value(),
						 aPastedSO.GetFatherComponent());
	aTargetLabel = DF_Label::Label(theDestinationStart, aDestEntry);
      } else
	theEngine->PasteInto(aStream, aLen, anObjID->Value(), aPastedSO);

      if(aStream != NULL) delete []aStream;
    }
  }

  // iterate attributes
  vector<DF_Attribute*> attrList = theSource.GetAttributes();
  for(int i = 0, len = attrList.size(); i<len; i++) {
    DF_Attribute* anAttr = attrList[i];
    if (aTargetLabel.FindAttribute(anAttr->ID())) {
      aTargetLabel.ForgetAttribute(anAttr->ID());
    }
    DF_Attribute* aNewAttribute = anAttr->NewEmpty();
    aTargetLabel.AddAttribute(aNewAttribute);
    anAttr->Paste(aNewAttribute);
  }

  // check auxiliary label for Comment => reference or name attribute of the referenced object
  SALOMEDSImpl_AttributeComment* aCommentAttribute = NULL;
  if ((aCommentAttribute=(SALOMEDSImpl_AttributeComment*)aAuxSourceLabel.FindAttribute(SALOMEDSImpl_AttributeComment::GetID()))) {
    char * anEntry = new char[aCommentAttribute->Value().size() + 1];
    strcpy(anEntry, string(aCommentAttribute->Value()).c_str());
    char* aNameStart = strchr(anEntry, ' ');
    if (aNameStart) {
      *aNameStart = '\0';
      aNameStart++;
    }
    if (theCopiedStudyID == theDestinationStudy->StudyId()) { // if copy to the same study, reanimate reference
      DF_Label aRefLabel = DF_Label::Label(aTargetLabel, anEntry);
      SALOMEDSImpl_AttributeReference::Set(aTargetLabel, aRefLabel);
      // target attributes structure support
      SALOMEDSImpl_AttributeTarget::Set(aRefLabel)->Add(SALOMEDSImpl_Study::SObject(aTargetLabel));
    } else {
      if (aNameStart) SALOMEDSImpl_AttributeName::Set(aTargetLabel, aNameStart);
      else SALOMEDSImpl_AttributeName::Set(aTargetLabel, std::string("Reference to:")+anEntry);
    }
    delete [] anEntry;
  }

  return aTargetLabel;
}

//============================================================================
/*! Function : Paste
 *  Purpose  :
 */
//============================================================================
SALOMEDSImpl_SObject SALOMEDSImpl_StudyManager::Paste(const SALOMEDSImpl_SObject& theObject,
					       SALOMEDSImpl_Driver* theEngine)
{
  _errorCode = "";

  SALOMEDSImpl_SObject so;
  SALOMEDSImpl_Study* aStudy = theObject.GetStudy();

  // if study is locked, then paste can't be done
  if (aStudy->GetProperties()->IsLocked()) {
    _errorCode = "LockProtection";
    throw LockProtection("LockProtection");
  }

  // if there is no component name, then paste only SObjects and attributes: without component help
  SALOMEDSImpl_AttributeComment* aComponentName = NULL;
  bool aStructureOnly = !(aComponentName=(SALOMEDSImpl_AttributeComment*)_clipboard->Main().Root().FindAttribute(SALOMEDSImpl_AttributeComment::GetID()));

  // get copied study ID
  SALOMEDSImpl_AttributeInteger* aStudyIDAttribute = NULL;
  if (!(aStudyIDAttribute=(SALOMEDSImpl_AttributeInteger*)_clipboard->Main().Root().FindAttribute(SALOMEDSImpl_AttributeInteger::GetID()))) {
    _errorCode = "No study ID was found";
    return so;
  }
  int aCStudyID = aStudyIDAttribute->Value();

  // CAF document of current study usage
  DF_Document* aDocument = GetDocumentOfStudy(aStudy);
  if (!aDocument) {
    _errorCode = "Document is null";
    return so;
  }

  SALOMEDSImpl_SComponent aComponent = theObject.GetFatherComponent();

  // fill root inserted SObject
  DF_Label aStartLabel;
  if (aStructureOnly) {
    DF_Label anObjectLabel = DF_Label::Label(aDocument->Main(), theObject.GetID());
    aStartLabel = PasteLabel(aStudy, theEngine, _clipboard->Main(), anObjectLabel, aCStudyID, false);
  } else {
    DF_Label aComponentLabel = DF_Label::Label(aDocument->Main(), aComponent.GetID());
    aStartLabel = PasteLabel(aStudy, theEngine, _clipboard->Main(), aComponentLabel, aCStudyID, true);
  }

  // paste all sublebels
  DF_ChildIterator anIterator(_clipboard->Main(), true);
  for(; anIterator.More(); anIterator.Next()) {
    PasteLabel(aStudy, theEngine, anIterator.Value(), aStartLabel, aCStudyID, false);
  }

  return SALOMEDSImpl_Study::SObject(aStartLabel);
}

//#######################################################################################################
//#                                     STATIC PRIVATE FUNCTIONS
//#######################################################################################################

//============================================================================
/*! Function : SaveAttributes
 *  Purpose  : Save attributes for object
 */
//============================================================================
static void SaveAttributes(const SALOMEDSImpl_SObject& aSO, HDFgroup *hdf_group_sobject)
{
  hdf_size size[1];
  vector<DF_Attribute*> attrList = aSO.GetLabel().GetAttributes();
  DF_Attribute* anAttr = NULL;
  for(int i = 0, len = attrList.size(); i<len; i++) {
    anAttr = attrList[i];
    //The following attributes are not supposed to be written to the file
    string type = SALOMEDSImpl_GenericAttribute::Impl_GetType(anAttr);
    if(type == string("AttributeIOR")) continue; //IOR attribute is not saved
    string aSaveStr =anAttr->Save();
    //cout << "Saving: " << aSO.GetID() << " type: "<< type<<"|" << endl;
    size[0] = (hdf_int32) strlen(aSaveStr.c_str()) + 1;
    HDFdataset *hdf_dataset = new HDFdataset((char*)type.c_str(), hdf_group_sobject, HDF_STRING,size, 1);
    hdf_dataset->CreateOnDisk();
    hdf_dataset->WriteOnDisk((char*)aSaveStr.c_str());
    hdf_dataset->CloseOnDisk();
    hdf_dataset=0; //will be deleted by hdf_sco_group destructor
  }
}

//===========================================================================
//Function : ReadAttributes
//===========================================================================
static void ReadAttributes(SALOMEDSImpl_Study* theStudy,
			   const SALOMEDSImpl_SObject& aSO,
			   HDFdataset* hdf_dataset)
{
  hdf_dataset->OpenOnDisk();

  DF_Attribute* anAttr = NULL;
  char* current_string = new char[hdf_dataset->GetSize()+1];
  hdf_dataset->ReadFromDisk(current_string);
  //cout << "Reading attr type = " << hdf_dataset->GetName() << "  SO = " << aSO.GetID() << endl;
  if (!strcmp(hdf_dataset->GetName(),"COMPONENTDATATYPE")) {
    anAttr = theStudy->NewBuilder()->FindOrCreateAttribute(aSO, "AttributeComment");
  } else if (!strcmp(hdf_dataset->GetName(),"AttributeReference") ||
             !strcmp(hdf_dataset->GetName(),"Reference")) { // Old format maintainance
    theStudy->NewBuilder()->Addreference(aSO, theStudy->CreateObjectID(current_string));
    delete(current_string);
    hdf_dataset->CloseOnDisk();
    return;
  } else {
    anAttr = theStudy->NewBuilder()->FindOrCreateAttribute(aSO, hdf_dataset->GetName());
  }
  
  if (anAttr) {
    anAttr->Load(current_string);
  }
  
  delete(current_string);
  hdf_dataset->CloseOnDisk();
}

//============================================================================
//Function : BuildlTree
//============================================================================
static void BuildTree (SALOMEDSImpl_Study* theStudy, HDFgroup* hdf_current_group)
{
  hdf_current_group->OpenOnDisk();
  SALOMEDSImpl_SObject aSO;
  char* Entry = hdf_current_group->GetName();
  if (strcmp(Entry,"STUDY_STRUCTURE") == 0) {
    aSO = theStudy->CreateObjectID("0:1");
  }
  else {
    aSO = theStudy->CreateObjectID(Entry);
  }

  char name[HDF_NAME_MAX_LEN+1];
  int nbsons = hdf_current_group->nInternalObjects();
  for (int i=0; i<nbsons; i++) {
    hdf_current_group->InternalObjectIndentify(i,name);
    if (strncmp(name, "INTERNAL_COMPLEX",16) == 0) continue;
    hdf_object_type type = hdf_current_group->InternalObjectType(name);

    if  (type == HDF_DATASET) {
      HDFdataset* new_dataset = new HDFdataset(name,hdf_current_group);
      ReadAttributes(theStudy,aSO,new_dataset);
      new_dataset = 0; // will be deleted by father destructor

    }
    else if (type == HDF_GROUP)   {
      HDFgroup* new_group = new HDFgroup(name,hdf_current_group);
      BuildTree (theStudy, new_group);
      new_group = 0; // will be deleted by father destructor
    }
  }
  hdf_current_group->CloseOnDisk();
}


//============================================================================
//Function : Translate_IOR_to_persistentID
//============================================================================
static void Translate_IOR_to_persistentID (const SALOMEDSImpl_SObject& so,
					   SALOMEDSImpl_Driver*                engine,
					   bool                                isMultiFile,
					   bool                                isASCII)
{
  DF_ChildIterator itchild(so.GetLabel());
  string ior_string,  persistent_string, curid;

  for (; itchild.More(); itchild.Next()) {
    SALOMEDSImpl_SObject current = SALOMEDSImpl_Study::SObject(itchild.Value());
    SALOMEDSImpl_AttributeIOR* IOR = NULL;
    if ((IOR=(SALOMEDSImpl_AttributeIOR*)current.GetLabel().FindAttribute(SALOMEDSImpl_AttributeIOR::GetID()))) {
      ior_string = IOR->Value();

      persistent_string = engine->IORToLocalPersistentID (current, ior_string, isMultiFile, isASCII);
      SALOMEDSImpl_AttributePersistentRef::Set(current.GetLabel(), persistent_string);
    }
    Translate_IOR_to_persistentID (current, engine, isMultiFile, isASCII);
  }
}

