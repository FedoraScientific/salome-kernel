//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
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
//  File   : SALOMEDS.cxx
//  Author : Sergey ANIKIN
//  Module : SALOME
//  $Header$


#ifndef WNT
#include <SALOMEDS.hxx>
#else
#include "SALOMEDS.hxx"
#endif

using namespace SALOMEDS;

// PAL8065: san -- Global recursive mutex for SALOMEDS methods
Utils_Mutex Locker::MutexDS;

// PAL8065: san -- Global SALOMEDS locker
Locker::Locker()
: Utils_Locker( &MutexDS )
{}

Locker::~Locker()
{}

void SALOMEDS::lock()
{
  Locker::MutexDS.lock();
}

void SALOMEDS::unlock()
{
  Locker::MutexDS.unlock();
}
