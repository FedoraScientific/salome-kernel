// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
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

/*  
   File   : CalciumInterface.hxx
   Author : Eric Fayolle (EDF)
   Module : KERNEL
   Modified by : $LastChangedBy$
   Date        : $LastChangedDate: 2007-03-01 13:27:58 +0100 (jeu, 01 mar 2007) $
   Id          : $Id$
*/
#ifndef _CALCIUM_C_INTERFACE_H_
#define _CALCIUM_C_INTERFACE_H_

#include "CalciumMacroCInterface.hxx"
#include "CalciumTypes.hxx"
#include "CalciumFortranInt.h"
#include <cstdlib>

/* D�claration de l'Interface entre l'API C et l'API C++
   L'utilisateur CALCIUM n'a normalement pas a utliser cette interface
   En C/C++ il utilisera celle d�finie dans Calcium.c (calcium.h)
2   En C++/CORBA directement celle de CalciumCxxInterface.hxx
*/

/* En CALCIUM l'utilisation de donn�es de type double
   implique des dates de type double, pour les autres
   types de donn�es les dates sont de type float
*/
template <class T> struct CalTimeType {
  typedef float TimeType;
};

template <> struct CalTimeType<double> {
  typedef double TimeType;
};

/* D�claration de ecp_lecture_... , ecp_ecriture_..., ecp_free_... */

/*  Le premier argument est utilis� :
    - comme suffixe dans la d�finition des noms ecp_lecture_ , ecp_ecriture_ et ecp_free_
    Le second argument est utilis� :
    - comme argument template � l'appel de la m�thode C++ correspondante
        ( le type CORBA de port correspondant est alors obtenu par un trait)
   Le troisi�me argument est utilis�e :
   - pour typer le param�tre data de la proc�dure g�n�r�e 
   - pour d�duire le type des param�tres t, ti tf via un trait
   - comme premier param�tre template � l'appel de la m�thode C++ correspondante
         (pour typer les donn�es pass�es en param�tre )
   Notons que dans le cas CALCIUM_C2CPP_INTERFACE_(int,int,), le type int n'existe pas
   en CORBA, le port CALCIUM correspondant utilise une s�quence de long. La m�thode
   C++ CALCIUM de lecture rep�re cette diff�rence de type et charge 
   le manipulateur de donn�es d'effectuer  une recopie (qui fonctionne si les types sont compatibles). 
   Notons qu'en CORBA CORBA:Long est mapp� sur long uniquement si celui-ci est 32bits sinon
   il sera mapp� sur le type int (si il est 32bits). Le type CORBA:LongLong est mapp� sur le type long
   s'il est 64 bits sinon cel� peut �tre un long long (s'il existe).
*/
CALCIUM_C2CPP_INTERFACE_HXX_(intc,int,int,);
CALCIUM_C2CPP_INTERFACE_HXX_(long,long,long,);

CALCIUM_C2CPP_INTERFACE_HXX_(integer,integer,cal_int,);
CALCIUM_C2CPP_INTERFACE_HXX_(int2integer,integer,int,);
CALCIUM_C2CPP_INTERFACE_HXX_(long2integer,integer, long,);

CALCIUM_C2CPP_INTERFACE_HXX_(float,float,float, );
CALCIUM_C2CPP_INTERFACE_HXX_(double,double,double,);

CALCIUM_C2CPP_INTERFACE_HXX_(float2double,double,float, );

/*  Fonctionne mais essai suivant pour simplification de Calcium.c CALCIUM_C2CPP_INTERFACE_(bool,bool,);*/
CALCIUM_C2CPP_INTERFACE_HXX_(bool,bool,int,);
CALCIUM_C2CPP_INTERFACE_HXX_(cplx,cplx,float,);
CALCIUM_C2CPP_INTERFACE_HXX_(str,str,char*,);

/* D�claration de ecp_fin */
extern "C" CalciumTypes::InfoType ecp_fin_ (void * component, int code);
extern "C" CalciumTypes::InfoType ecp_cd_ (void * component, char* instanceName);
extern "C" CalciumTypes::InfoType ecp_fini_ (void * component, char* nomVar, int i);
extern "C" CalciumTypes::InfoType ecp_fint_ (void * component, char* nomVar, float t);
extern "C" CalciumTypes::InfoType ecp_effi_ (void * component, char* nomVar, int i);
extern "C" CalciumTypes::InfoType ecp_efft_ (void * component, char* nomVar, float t);

#endif
