//  SALOME Logger : CORBA server managing trace output
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : SALOME_Logger_Server.hxx
//  Author : Vasily Rusyaev
//  Module : SALOME

#if !defined SALOME_Logger_Server_include
#define SALOME_Logger_Server_include

#ifndef WNT
#include <fstream.h>
#else
#include <fstream>
#include <iosfwd>
#endif
#include <omnithread.h>
#include "Logger.hh"

class Logger :
  public POA_SALOME_Logger::Logger,
  public PortableServer::RefCountServantBase 
{
public:
        //constructor w/o parameters
        //all messages will be put into terminal via cout
	Logger();
        //constructor with parameter, filename is output file
        //all messages will be put into special file passed as parameter
	Logger(const char *filename);
	virtual ~Logger();
	//put message into one special place for all servers
	void putMessage(const char* message);
        void ping();
private:
	//if m_putIntoFile is true all messages will be put into special 
	//otherwise all messages will be put into terminal via cout 
        bool m_putIntoFile;
	//ofstream class specialized for disk file output
#ifndef WNT
        ofstream m_outputFile; 
#else
		std::ofstream m_outputFile; 
#endif
	//synchronisation object
	static omni_mutex myLock;
};

#endif // !defined(SALOME_Logger_Server_include)
