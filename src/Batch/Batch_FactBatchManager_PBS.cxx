// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public  
// License along with this library; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
/*
 * FactBatchManager_PBS.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2004
 * Projet : SALOME 2
 *
 */

#include <string>
#include "Batch_BatchManager_PBS.hxx"
#include "Batch_FactBatchManager_PBS.hxx"
#include "utilities.h"
using namespace std;

namespace Batch {

  static FactBatchManager_PBS sFBM_PBS;

  // Constructeur
  FactBatchManager_PBS::FactBatchManager_PBS() : FactBatchManager("PBS")
  {
    // Nothing to do
  }

  // Destructeur
  FactBatchManager_PBS::~FactBatchManager_PBS()
  {
    // Nothing to do
  }

  // Functor
  BatchManager * FactBatchManager_PBS::operator() (const char * hostname) const
  {
    MESSAGE("Building new BatchManager_PBS on host '" << hostname << "'");
    return new BatchManager_PBS(this, hostname);
  }


}
