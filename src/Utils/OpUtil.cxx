//  SALOME Utils : general SALOME's definitions and tools
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
//  File   : OpUtil.cxx
//  Module : SALOME

#include "utilities.h" 
#include "OpUtil.hxx"
#include <unistd.h>
#include <errno.h>
#include <string.h>
using namespace std;

int gethostname(char *name, size_t len);

string GetHostname()
{
  int ls = 100, r = 0;
  char *s;

  while (ls < 10000) {
    ls *= 2;
    s = new char[ls];
    r = gethostname(s, ls-1);
    switch (r) 
      {
      case 0:
	  break;
      default:
#ifdef EINVAL
      case EINVAL:
#endif
#ifdef ENAMETOOLONG
      case ENAMETOOLONG:
#endif
        delete [] s;
	continue;
      }
  }

  if (r != 0) {
    s = new char[50];
    strcpy(s, "localhost");
  }

  // remove all after '.'
  char *aDot = (strchr(s,'.'));
  if (aDot) aDot[0] = '\0';

  string p = s;
  delete [] s;
  return p;
}

