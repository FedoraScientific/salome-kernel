// Copyright (C) 2006  OPEN CASCADE, CEA/DEN, EDF R&D, PRINCIPIA R&D
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
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//
//
//  File   : SALOME_FileTransferCORBA.hxx
//  Author : Paul RASCLE, EDF
//  Module : SALOME
//  $Header$

#ifndef _SALOME_FILETRANSFERCORBA_HXX_
#define _SALOME_FILETRANSFERCORBA_HXX_


#include <SALOMEconfig.h>
#include <Utils_SALOME_Exception.hxx>

#include CORBA_CLIENT_HEADER(SALOME_Component)

#include <string>

class SALOME_FileTransferCORBA
{
public:
  SALOME_FileTransferCORBA();
  SALOME_FileTransferCORBA(Engines::fileRef_ptr aFileRef);
  SALOME_FileTransferCORBA(std::string refMachine,
			   std::string origFileName,
			   std::string containerName="");

  virtual ~SALOME_FileTransferCORBA();

  std::string getLocalFile(std::string localFile = "");

protected:
  Engines::fileRef_var _theFileRef;
  std::string _refMachine;
  std::string _origFileName;
  std::string _containerName;
};

#endif
