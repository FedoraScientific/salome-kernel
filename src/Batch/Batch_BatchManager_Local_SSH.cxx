/*
 * BatchManager_Local_SSH.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Thu Nov  6 10:17:22 2003
 * Projet : Salome 2
 *
 */

#ifdef HAVE_CONFIG_H
#  include <SALOMEconfig.h>
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctime>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include "Batch_IOMutex.hxx"
#include "Batch_BatchManager_Local_SSH.hxx"

#ifndef RM
#error "RM undefined. You must set RM to a valid path to a rm-like command."
#endif

#ifndef RCP
#error "RCP undefined. You must set RCP to a valid path to a scp-like command."
#endif

#ifndef SSH
#error "SSH undefined. You must set SSH to a valid path to a ssh-like command."
#endif

namespace Batch {


  // Constructeur
  BatchManager_Local_SSH::BatchManager_Local_SSH(const FactBatchManager * parent, const char * host) throw(InvalidArgumentException,ConnexionFailureException) : BatchManager_Local(parent, host)
  {
  }

  // Destructeur
  BatchManager_Local_SSH::~BatchManager_Local_SSH()
  {
  }


  // Methode abstraite qui renvoie la commande de copie du fichier source en destination
  string BatchManager_Local_SSH::copy_command(const string & host_source, const string & source, const string & host_destination, const string & destination) const
  {
    ostringstream fullsource;
    if (host_source.size() == 0) {
      fullsource << "localhost:";
    } else {
      fullsource << host_source << ":";
    }
    fullsource << source;

    ostringstream fulldestination;
    if (host_destination.size() == 0) {
      fulldestination << "localhost:";
    } else {
      fulldestination << host_destination << ":";
    }
    fulldestination << destination;

    ostringstream copy_cmd;
    copy_cmd << RCP << " " << fullsource.str() << " " << fulldestination.str();
    return copy_cmd.str();
  }
  
  // Methode abstraite qui renvoie la commande a executer
  string BatchManager_Local_SSH::exec_command(Parametre & param) const
  {
    ostringstream exec_sub_cmd;
    exec_sub_cmd << param[EXECUTABLE];

    if (param.find(ARGUMENTS) != param.end()) {
      Versatile V = param[ARGUMENTS];
      for(Versatile::const_iterator it=V.begin(); it!=V.end(); it++) {
	StringType argt = * static_cast<StringType *>(*it);
	string     arg  = argt;
	exec_sub_cmd << " " << arg;
      }
    }


    Versatile new_arguments;
    new_arguments.setMaxSize(0);
    new_arguments = string(param[EXECUTIONHOST]);


    if (param.find(USER) != param.end()) {
      new_arguments += "-l";
      new_arguments += string(param[USER]);
    }

    new_arguments += exec_sub_cmd.str();

    param[ARGUMENTS] = new_arguments;

    // Sous Linux on est oblige de modifier ces deux parametres pour faire fonctionner la commande rsh
    param[EXECUTABLE] = SSH;
    param.erase(NAME);

    return SSH;
  }

  // Methode qui renvoie la commande d'effacement du fichier
  string BatchManager_Local_SSH::remove_command(const string & host_destination, const string & destination) const
  {
    string host = (host_destination.size()) ? host_destination : "localhost:";

    ostringstream remove_cmd;
    remove_cmd << SSH << " " << host << " \"" << RM << " " << destination << "\"";
    return remove_cmd.str();
  }
}
