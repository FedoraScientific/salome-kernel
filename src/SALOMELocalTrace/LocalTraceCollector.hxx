//  Copyright (C) 2004  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  File   : LocalTraceCollector.hxx
//  Author : Paul RASCLE (EDF)
//  Module : KERNEL
//  $Header$

#ifndef _LOCALTRACECOLLECTOR_HXX_
#define _LOCALTRACECOLLECTOR_HXX_

#include <string>
#include "LocalTraceBufferPool.hxx"

//! See SALOMETraceCollector instead of LocalTraceCollector for SALOME usage

class LocalTraceCollector
{
 public:
  static LocalTraceCollector* instance(int typeTrace=0);
  static void *run(void *bid);
  ~LocalTraceCollector();

 protected:
  LocalTraceCollector();

 private:
  static int _threadToClose;
  static int _toFile;
  static LocalTraceCollector* _singleton;
  static pthread_mutex_t _singletonMutex;
  static pthread_t _threadId;
  static std::string _fileName;
};

#endif
