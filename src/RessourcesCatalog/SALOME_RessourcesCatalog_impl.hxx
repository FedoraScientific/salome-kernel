// File: SALOME_RessourcesCatalog_impl.hxx
// Created: Mon Sep 10 2001
// Author: Estelle Deville
// Project: SALOME
// Copyright : CEA/DEN/DMSS/LGLS
// $Header$


#ifndef RESSOURCESCATALOG_IMPL_H
#define RESSOURCESCATALOG_IMPL_H

#include "utilities.h"
#include "SALOME_RessourcesCatalog_Handler.hxx"
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_RessourcesCatalog)

class SALOME_RessourcesCatalogImpl: public POA_SALOME_RessourcesCatalog::RessourcesCatalog,
				  public PortableServer::RefCountServantBase 
{
public:
  //! standard constructor
  SALOME_RessourcesCatalogImpl(int argc, char** argv);

  //! standard destructor
  virtual ~SALOME_RessourcesCatalogImpl();

  //! method to get the ressources list
  /*!
    \return a computer list 
  */
  virtual SALOME_RessourcesCatalog::ListOfComputer* GetComputerList();

  //! method to get the container type list of a computer
  /*!
    \param computer const char* arguments
    \return a container type list 
  */
  virtual SALOME_RessourcesCatalog::ListOfContainerType* GetContainerTypeList(const char* computer) throw(SALOME_RessourcesCatalog::NotFound);

  //! method to get informations on a computer
  /*!
    \param computer const char* arguments
    \return informations of the computer
  */
  virtual SALOME_RessourcesCatalog::computer_info* GetComputerInfo(const char* computer) throw(SALOME_RessourcesCatalog::NotFound);


private:

  //! method to parse the data type catalog
  /*! 
    \param file const char* arguments
    \param datatypelist ListOfParserDataType arguments
  */
  virtual void _parse_xml_file(const char* file, 
			       ListOfParserressources& ressourceslist);


 //! method to verify ressources catalog content
  /*!
    \param ressourceslist ListOfParserRessources arguments
    \return true if verfication is OK
  */
  virtual bool _verify_ressources(ListOfParserressources ressourceslist);


 //! method to parse arguments to get catalog file
  /*!
    \param argc int arguments
    \param argv char** arguments
    \param _path char** arguments
    \return true if parsing is OK
  */
  virtual bool _parseArguments(int argc, char **argv, char **_path);

 //! method to create a list of processors information from the catalog parsing
  /*!
    \param list_procs ListOfParserProc arguments
    \return the processors information list
  */
  virtual SALOME_RessourcesCatalog::ListOfProc
          _duplicate_procs(ListOfParserProc list_procs);


  // will contain the path to the ressources catalog
  char* _path_ressources;


  //will contain the informations on the data type catalog(after parsing)
  ListOfParserressources _ressourceslist ;
};

#endif // RESSOURCESCATALOG_IMPL_H
