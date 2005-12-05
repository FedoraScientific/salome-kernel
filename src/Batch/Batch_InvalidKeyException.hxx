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
// See http://www.salome-platform.org/
//
/*
 * InvalidKeyException.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Wed Oct 15 10:39:51 2003
 * Projet : Salome 2
 *
 */

#ifndef _INVALIDKEYEXCEPTION_H_
#define _INVALIDKEYEXCEPTION_H_


#include "Batch_GenericException.hxx"
#include "Batch_GenericException.hxx"

namespace Batch {

  class InvalidKeyException : public GenericException
  {
  public:
		// Constructeur
    InvalidKeyException(std::string ch = "undefined") : GenericException("InvalidKeyException", ch) {}

  protected:

  private:

  };

}

#endif

