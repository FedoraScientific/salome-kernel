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



  File   : HDFattrGetType.c
Module : SALOME
----------------------------------------------------------------------------*/

#include "hdfi.h"
#include <hdf5.h>

hdf_type
HDFattrGetType(hdf_idt id)
{
  hdf_idt type_id;
  hdf_type type;
  hdf_class_type hdf_type;
  hdf_size_type size;

  if ((type_id = H5Aget_type(id)) < 0)
    return HDF_NONE;

  hdf_type = H5Tget_class(type_id);

  switch (hdf_type)
    {
    case H5T_INTEGER :
      size = H5Tget_size(type_id);
      if (size == 4)
	type = HDF_INT32;
      else
	type = HDF_INT64;
      break;

    case H5T_FLOAT :
      type = HDF_FLOAT64;
      break;

    case H5T_STRING :
      type = HDF_STRING;
      break;

    default :
      type = HDF_NONE;
    }

  H5Tclose(type_id);

  return type;
}
