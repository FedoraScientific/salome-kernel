/*----------------------------------------------------------------------------
SALOME HDFPersist : implementation of HDF persitent ( save/ restore )

 Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
 CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 

 This library is free software; you can redistribute it and/or 
 modify it under the terms of the GNU Lesser General Public 
 License as published by the Free Software Foundation; either 
 version 2.1 of the License. 

 This library is distributed in the hope that it will be useful, 
 but WITHOUT ANY WARRANTY; without even the implied warranty of 
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 Lesser General Public License for more details. 

 You should have received a copy of the GNU Lesser General Public 
 License along with this library; if not, write to the Free Software 
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 

 See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 



  File   : HDFfileCreate.c
  Module : SALOME
----------------------------------------------------------------------------*/

#include "hdfi.h"

/*
 * - Name : HDFfileCreate
 * - Description : creates a HDF file
 * - Parameters :
 *     - name (IN) : file name
 * - Result : 
 *     - success : file ID
 *     - failure : -1 
 */ 
hdf_idt HDFfileCreate(char *name)
{
  hdf_idt fid;

  if ((fid = H5Fcreate(name,H5F_ACC_TRUNC,
			       H5P_DEFAULT,H5P_DEFAULT)) < 0)
    return -1;

  return fid;
}
