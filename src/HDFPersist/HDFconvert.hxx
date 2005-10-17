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
//  File   : HDFconvert.hxx
//  Module : SALOME

#ifndef HDFCONVERT_HXX
#define HDFCONVERT_HXX

extern "C"
{
#include "HDFtypes.h"
#ifndef WNT
#include <unistd.h>
#include <sys/mman.h>
#else
#endif
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
}
#include "HDFcontainerObject.hxx"
#include "HDFdataset.hxx"
#include "HDFfile.hxx"
#include <string>


class HDFConvert 
{
private:

  HDFConvert();
  ~HDFConvert();

public:

static int FromAscii(const std::string& file, const  HDFcontainerObject& hdf_container, const std::string& nomdataset);

};

#endif /* HDFCONVERT_HXX */ 
