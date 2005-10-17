#include "SALOMEMultiComm.hxx"
#ifndef WNT
using namespace std;
#endif

SALOMEMultiComm::SALOMEMultiComm():_type(SALOME::CORBA_)
{
}

SALOMEMultiComm::SALOMEMultiComm(SALOME::TypeOfCommunication type):_type(type)
{
}

void SALOMEMultiComm::setProtocol(SALOME::TypeOfCommunication type)
{
  _type=type;
}

SALOME::TypeOfCommunication SALOMEMultiComm::getProtocol() const
{
  return _type;
}


