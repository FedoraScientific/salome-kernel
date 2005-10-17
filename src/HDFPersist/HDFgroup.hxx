//  SALOME HDFPersist : implementation of HDF persitent ( save/ restore )
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
//  File   : HDFgroup.hxx
//  Module : SALOME

#ifndef HDFGROUP_HXX
#define HDFGROUP_HXX

extern "C"
{
#include "HDFtypes.h"
}
#include "HDFcontainerObject.hxx"
#include <Standard_Macro.hxx>

class Standard_EXPORT HDFgroup : public HDFcontainerObject
{
private :
  HDFcontainerObject *_father;
  hdf_idt _fid;
  hdf_idt _mid;
  char* _attribute;
public :
  HDFgroup(char *name, HDFcontainerObject *father);

  void CreateOnDisk();
  void OpenOnDisk();
  void CloseOnDisk();

  void FileMount(char *file,hdf_access_mode mode);
  void FileUnMount();

  HDFcontainerObject *GetFather();
  hdf_object_type GetObjectType();

  int nInternalObjects();
  void InternalObjectIndentify(int rank, char *object_name);
  int ExistInternalObject(char *object_name);
  hdf_object_type InternalObjectType(char *object_name);

  int nAttributes();
  char* GetAttributeName(unsigned idx);
};

#endif /* HDFGROUP_HXX */ 
