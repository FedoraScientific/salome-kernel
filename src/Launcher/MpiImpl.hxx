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
 * BatchManager.hxx : 
 *
 * Auteur : Bernard SECHER - CEA/DEN
 * Date   : Juillet 2007
 * Projet : SALOME
 *
 */

#ifndef _BL_MPIIMPL_H_
#define _BL_MPIIMPL_H_

#include <string>
#include "Utils_SALOME_Exception.hxx"
#include <SALOMEconfig.h>

class MpiImpl
{
public:
  // Constructeur et destructeur
  MpiImpl(); // constrcuctor
  virtual ~MpiImpl(); //Destructor
  
  virtual std::string size() = 0; // get number of process of current job
  virtual std::string rank() = 0; // get process number of current job
  virtual std::string boot(const std::string machinefile, const unsigned int nbnodes) = 0; // get boot command
  virtual std::string run(const std::string machinefile, const unsigned int nbproc, const std::string fileNameToExecute) = 0; // get run command
  virtual std::string halt() = 0; // get stop command

protected:

private:

};

class MpiImpl_LAM : public MpiImpl
{
public:
  // Constructeur et destructeur
  MpiImpl_LAM(); // constructor
  virtual ~MpiImpl_LAM(); //Destructor

  std::string size(); // get number of process of current job
  std::string rank(); // get process number of current job
  std::string boot( const std::string machinefile, const unsigned int nbnodes); // get boot command
  std::string run( const std::string machinefile, const unsigned int nbproc, const std::string fileNameToExecute); // get run command
  std::string halt(); // get stop command

protected:
  
private:

};

class MpiImpl_MPICH1 : public MpiImpl
{
public:
  // Constructeur et destructeur
  MpiImpl_MPICH1(); // constructor
  virtual ~MpiImpl_MPICH1(); //Destructor
  
  std::string size(); // get number of process of current job
  std::string rank(); // get process number of current job
  std::string boot( const std::string machinefile, const unsigned int nbnodes); // get boot command
  std::string run( const std::string machinefile, const unsigned int nbproc, const std::string fileNameToExecute); // get run command
  std::string halt(); // get stop command

protected:
  
private:

};

class MpiImpl_MPICH2 : public MpiImpl
{
public:
  // Constructeur et destructeur
  MpiImpl_MPICH2(); // constructor
  virtual ~MpiImpl_MPICH2(); //Destructor

  std::string size(); // get number of process of current job
  std::string rank(); // get process number of current job
  std::string boot( const std::string machinefile, const unsigned int nbnodes); // get boot command
  std::string run( const std::string machinefile, const unsigned int nbproc, const std::string fileNameToExecute); // get run command
  std::string halt(); // get stop command

protected:

private:

};

class MpiImpl_OPENMPI : public MpiImpl
{
public:
  // Constructeur et destructeur
  MpiImpl_OPENMPI(); // constructor
  virtual ~MpiImpl_OPENMPI(); //Destructor

  std::string size(); // get number of process of current job
  std::string rank(); // get process number of current job
  std::string boot( const std::string machinefile, const unsigned int nbnodes); // get boot command
  std::string run( const std::string machinefile, const unsigned int nbproc, const std::string fileNameToExecute); // get run command
  std::string halt(); // get stop command

protected:

private:

};

#endif
