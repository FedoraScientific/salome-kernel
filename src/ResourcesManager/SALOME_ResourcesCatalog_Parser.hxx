//  SALOME ResourcesCatalog : implementation of catalog resources parsing (SALOME_ModuleCatalog.idl)
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
//  File   : SALOME_ResourcesCatalog_Parser.hxx
//  Author : Estelle Deville
//  Module : SALOME
//$Header$

#ifndef SALOME_RESOURCES_CATALOG_PARSER
#define SALOME_RESOURCES_CATALOG_PARSER

#include <string>
#include <vector>
#include <map>
#include <iostream>

typedef std::map<std::string, std::string> MapOfModulesPath;

enum AccessProtocolType {rsh, ssh};

enum AccessModeType {interactive, batch};

class ResourceDataToSort {
public:
  std::string _hostName;
  unsigned int _nbOfNodes;
  unsigned int _nbOfProcPerNode;
  unsigned int _CPUFreqMHz;
  unsigned int _memInMB;
  static unsigned int _nbOfNodesWanted;
  static unsigned int _nbOfProcPerNodeWanted;
  static unsigned int _CPUFreqMHzWanted;
  static unsigned int _memInMBWanted;
public:
  ResourceDataToSort();
  ResourceDataToSort(const std::string& hostname,unsigned int nbOfNodes,unsigned int nbOfProcPerNode,unsigned int CPUFreqMHz,unsigned int memInMB);
  bool operator< (const ResourceDataToSort& other) const;
  void Print() const;
private:
  unsigned int GetNumberOfPoints() const;
//   friend class SALOME_ResourcesCatalog_Handler;
//   friend class SALOME_ResourcesManager;
//   friend struct ParserResourcesType;
};

struct ParserResourcesType {
  ResourceDataToSort DataForSort;
  std::string Alias;
  AccessProtocolType Protocol;
  AccessModeType Mode;
  std::string UserName;
  MapOfModulesPath ModulesPath;
  std::string PreReqFilePath;
  std::string OS;

  void Print(){
    std::cout << "##############*****" << std::endl; 
    std::cout << "HostName : " << DataForSort._hostName << std::endl;
    std::cout << "Alias : " << Alias << std::endl;
    std::cout << "Protocol : " << Protocol << std::endl;
    std::cout << "Mode : " << Mode << std::endl;
    std::cout << "UserName : " << UserName << std::endl;
    std::cout << "Modules : " << std::endl;
    int i=1;
    for(std::map<std::string, std::string>::iterator iter=ModulesPath.begin();iter!=ModulesPath.end();iter++)
	{
	  std::cout << "    Module " << i++ << " called : " << (*iter).first << " with path : " << (*iter).second << std::endl;
	}
    std::cout << "PreReqFilePath : " << PreReqFilePath << std::endl;
    std::cout << "OS : " << OS << std::endl;
    DataForSort.Print();
  }
};

typedef std::map<std::string, ParserResourcesType> MapOfParserResourcesType;

#endif //SALOME_RESOURCES_CATALOG_PARSER