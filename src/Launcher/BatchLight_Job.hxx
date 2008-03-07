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
 * Job.hxx : 
 *
 * Auteur : Bernard SECHER - CEA/DEN
 * Date   : Juillet 2007
 * Projet : SALOME
 *
 */

#ifndef _BL_JOB_H_
#define _BL_JOB_H_

#include "utilities.h"
#include <SALOMEconfig.h>
#include CORBA_CLIENT_HEADER(SALOME_ContainerManager)

namespace BatchLight {

  class Job
  {
  public:
    // Constructeurs et destructeur
    Job(const char *fileToExecute, 
	const Engines::FilesList& filesToExport, 
	const Engines::FilesList& filesToImport, 
	const Engines::BatchParameters& batch_params);
    virtual ~Job();

    const char *getFileToExecute() const { return _fileToExecute; }
    const Engines::FilesList getFilesToExportList() const { return _filesToExport; }
    const Engines::FilesList getFilesToImportList() const { return _filesToImport; }
    void addFileToImportList(std::string file_name);
    const CORBA::Long getNbProc() const { return _batch_params.nb_proc; }
    const std::string getExpectedDuringTime();
    const std::string getMemory();

    const std::string getDirForTmpFiles() const { return _dirForTmpFiles;}
    void setDirForTmpFiles(std::string dirForTmpFiles) {_dirForTmpFiles = dirForTmpFiles;
							SCRUTE(_dirForTmpFiles);}
    bool check();							
  protected:
    const char* _fileToExecute;
    const Engines::FilesList _filesToExport;
    Engines::FilesList _filesToImport;
    Engines::BatchParameters _batch_params;
    std::string _dirForTmpFiles; // Tmp directory on the server
  private:

  };

}

#endif
