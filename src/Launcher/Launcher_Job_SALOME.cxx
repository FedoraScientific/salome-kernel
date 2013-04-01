// Copyright (C) 2009-2013  CEA/DEN, EDF R&D, OPEN CASCADE
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
//
// This library is distributed in the hope that it will be useful,
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

// Author: André RIBES - EDF R&D
//
#include "Launcher_Job_SALOME.hxx"
#include "Basics_DirUtils.hxx"

#ifdef WITH_LIBBATCH
#include <libbatch/Constants.hxx>
#endif

#ifdef WNT
#include <io.h>
#define _chmod chmod
#endif

Launcher::Job_SALOME::Job_SALOME() {}

Launcher::Job_SALOME::~Job_SALOME() {}

void 
Launcher::Job_SALOME::setResourceDefinition(const ParserResourcesType & resource_definition)
{
  // Check resource_definition
  if (resource_definition.AppliPath == "")
  {
    std::string mess = "Resource definition must define an application path !, resource name is: " + resource_definition.Name;
    throw LauncherException(mess);
  }
  Launcher::Job::setResourceDefinition(resource_definition);
}

void
Launcher::Job_SALOME::update_job()
{
#ifdef WITH_LIBBATCH
  Batch::Parametre params = common_job_params();
  params[Batch::EXECUTABLE] = buildSalomeScript(params);
  params[Batch::EXCLUSIVE] = true;
  _batch_job->setParametre(params);
#endif
}

#ifdef WITH_LIBBATCH
std::string 
Launcher::Job_SALOME::buildSalomeScript(Batch::Parametre params)
{
  // parameters
  std::string work_directory = params[Batch::WORKDIR].str();

  std::string launch_script = Kernel_Utils::GetTmpDir() + "runSalome_" + _job_file_name + "_" + _launch_date + ".sh";
  std::ofstream launch_script_stream;
  launch_script_stream.open(launch_script.c_str(), 
                            std::ofstream::out
#ifdef WIN32		
 | std::ofstream::binary   //rnv: to avoid CL+RF end of line on windows
#endif
                           );
   
  // Begin of script
  launch_script_stream << "#!/bin/sh -f" << std::endl;
  launch_script_stream << "cd " << work_directory << std::endl;
  launch_script_stream << "export PYTHONPATH=" << work_directory << ":$PYTHONPATH" << std::endl;
  launch_script_stream << "export PATH=" << work_directory << ":$PATH" << std::endl;
  if (_env_file != "")
  {
    std::string::size_type last = _env_file.find_last_of("/");
    launch_script_stream << ". " << _env_file.substr(last+1) << std::endl;
  }
  launch_script_stream << "export SALOME_TMP_DIR=" << work_directory << "/logs" << std::endl;

  // -- Generates Catalog Resources
  std::string resource_protocol = _resource_definition.getClusterInternalProtocolStr();
  launch_script_stream << "if [ \"x$LIBBATCH_NODEFILE\" != \"x\" ]; then " << std::endl;
  launch_script_stream << "CATALOG_FILE=" << "CatalogResources_" << _launch_date << ".xml" << std::endl;
  launch_script_stream << "export USER_CATALOG_RESOURCES_FILE=" << "$CATALOG_FILE" << std::endl;
  launch_script_stream << "echo '<!DOCTYPE ResourcesCatalog>'  > $CATALOG_FILE" << std::endl;
  launch_script_stream << "echo '<resources>'                 >> $CATALOG_FILE" << std::endl;	
  launch_script_stream << "cat $LIBBATCH_NODEFILE | sort | uniq -c | while read nbproc host"  << std::endl;
  launch_script_stream << "do"                                                  << std::endl;
  launch_script_stream << "echo '<machine hostname='\\\"$host\\\"			          >> $CATALOG_FILE" << std::endl;
  launch_script_stream << "echo '         protocol=\"" << resource_protocol               << "\"' >> $CATALOG_FILE" << std::endl;
  launch_script_stream << "echo '         userName=\"" << _resource_definition.UserName   << "\"' >> $CATALOG_FILE" << std::endl;
  launch_script_stream << "echo '         appliPath=\"" << _resource_definition.AppliPath << "\"' >> $CATALOG_FILE" << std::endl;
  launch_script_stream << "echo '         mpi=\"" << _resource_definition.getMpiImplTypeStr() << "\"' >> $CATALOG_FILE" << std::endl;
  launch_script_stream << "echo '         nbOfNodes='\\\"$nbproc\\\" >> $CATALOG_FILE" << std::endl;
  launch_script_stream << "echo '         nbOfProcPerNode=\"1\"' >> $CATALOG_FILE" << std::endl;
  launch_script_stream << "echo '         can_run_containers=\"true\"' >> $CATALOG_FILE" << std::endl;
  launch_script_stream << "echo '/>'                                                              >> $CATALOG_FILE" << std::endl;
  launch_script_stream << "done"                                 << std::endl;
  launch_script_stream << "echo '</resources>' >> $CATALOG_FILE" << std::endl;
  launch_script_stream << "fi" << std::endl;

  // Create file for ns-port-log
  launch_script_stream << "NS_PORT_FILE_PATH=`mktemp " << _resource_definition.AppliPath << "/USERS/nsport_XXXXXX` &&\n";
  launch_script_stream << "NS_PORT_FILE_NAME=`basename $NS_PORT_FILE_PATH` &&\n";

  // Launch SALOME with an appli
  launch_script_stream << _resource_definition.AppliPath << "/runAppli --terminal --ns-port-log=$NS_PORT_FILE_NAME --server-launch-mode=fork ";
  launch_script_stream << "> logs/salome_" << _launch_date << ".log 2>&1 &&" << std::endl;
  launch_script_stream << "current=0 &&\n"
                       << "stop=20 &&\n"
                       << "while ! test -s $NS_PORT_FILE_PATH\n"
                       << "do\n"
                       << "  sleep 2\n"
                       << "  current=$((current+1))\n"
                       << "  if [ \"$current\" -eq \"$stop\" ] ; then\n"
                       << "    echo Error Naming Service failed ! >&2\n"
                       << "    exit\n"
                       << "  fi\n"
                       << "done &&\n"
                       << "appli_port=`cat $NS_PORT_FILE_PATH` &&\n"
                       << "rm $NS_PORT_FILE_PATH &&\n";

  // Call real job type
  addJobTypeSpecificScript(launch_script_stream);

  // End
  launch_script_stream << _resource_definition.AppliPath << "/runSession -p $appli_port shutdownSalome.py" << std::endl;
  launch_script_stream << "sleep 10" << std::endl;

  // Return
  launch_script_stream.flush();
  launch_script_stream.close();
  chmod(launch_script.c_str(), 0x1ED);
  return launch_script;
}
#endif

