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
//  File   : HDFobject.cc
//  Module : SALOME

#include "HDFobject.hxx"
extern "C"
{
#include "hdfi.h"
#include <string.h>
}
#include "utilities.h"
using namespace std;

HDFobject::HDFobject(char *name)
{
  //  MESSAGE("-------- constructor " << name << " " << this);
  HDFerrorModeLock();
  _name = new char[strlen(name)+1];
  strcpy(_name,name);
  _id = -1;
}

HDFobject::~HDFobject()
{
  //delete [] _name;
  delete _name;
  //  MESSAGE("-------- destructor ------" << this);
}

hdf_idt HDFobject::GetId()
{
  return _id;
}

char *HDFobject::GetName()
{
  return _name;
}

hdf_object_type HDFobject::GetObjectType()
{
  return HDF_OBJECT;
}
