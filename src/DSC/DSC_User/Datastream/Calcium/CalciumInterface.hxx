//  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  File   : CalciumInterface.hxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-03-01 13:27:58 +0100 (jeu, 01 mar 2007) $
// Id          : $Id$
//
#ifndef _CALCIUM_INTERFACE_HXX_
#define _CALCIUM_INTERFACE_HXX_

//Interface CALCIUM des utilisateurs en C++ 
#include "CalciumCxxInterface.hxx"

#include "CalciumException.hxx"
#include "CalciumTypes.hxx"

#include <stdio.h>

//Ce fichier d�clare et d�fini l'interfa�age entre l'API utilisteur C et C++
//Les proc�dures d�clar�es n'ont pas vocation � �tre utilis�es directement (cel� est
// cependant possible).  
//#define _DEBUG_

#ifdef _DEBUG_
#define DEBTRACE(msg) {std::cerr<<std::flush<<__FILE__<<" ["<<__LINE__<<"] : "<<msg<<std::endl<<std::flush;}
#else
#define DEBTRACE(msg)
#endif


// Interface C/C++
// L'utilisateur CALCIUM n'a normalement pas a utliser cette interface
// En C/C++ il utilisera celle d�finie dans Calcium.c
// En C++/CORBA directement celle de CalciumCxxInterface

// En CALCIUM l'utilisation de donn�es de type double
// implique des dates de type double, pour les autres
// types de donn�es les dates sont de type float
template <class T> struct CalTimeType {
  typedef float TimeType;
};

template <> struct CalTimeType<double> {
  typedef double TimeType;
};

// D�finition de ecp_fin
extern "C"  CalciumTypes::InfoType 
ecp_fin_ (void * component, int code) {

  Superv_Component_i * _component = static_cast<Superv_Component_i *>(component); 

  bool provideLastGivenValue = false;
  if (code == CalciumTypes::CP_CONT ) provideLastGivenValue = true;

  try {									
    CalciumInterface::ecp_fin( *_component,				
			       provideLastGivenValue); 
  } catch ( const CalciumException & ex) { //tester l'arr�t par exception
    DEBTRACE( ex.what() );
    return ex.getInfo();						
  }									
  return CalciumTypes::CPOK;
};


// D�finition de ecp_lecture_... , ecp_ecriture_..., ecp_free_...
#define CALCIUM_C2CPP_INTERFACE_(_name,_type,_qual)			\
  extern "C" CalciumTypes::InfoType ecp_lecture_##_name (void * component, int dependencyType, \
							 CalTimeType< _type _qual >::TimeType * ti, \
							 CalTimeType< _type _qual >::TimeType * tf, long * i, \
							 const char * const nomvar, size_t bufferLength, \
							 size_t * nRead, _type _qual ** data ) { \
    Superv_Component_i * _component = static_cast<Superv_Component_i *>(component); \
    double         _ti=*ti;						\
    double         _tf=*tf;						\
    size_t         _nRead=0;						\
    size_t         _bufferLength=bufferLength;				\
    									\
    if ( IsSameType< _name , cplx >::value ) _bufferLength*=2;		\
    DEBTRACE( "-------- CalciumInterface(lecture Inter Part) MARK 1 ------------------" ) \
    try {								\
      CalciumInterface::ecp_lecture< _type, _name >( *_component,	\
						     dependencyType,	\
						     _ti, _tf, *i,	\
						     nomvar,		\
						     _bufferLength, _nRead, *data); \
    } catch ( const CalciumException & ex) {				\
      DEBTRACE( ex.what() );						\
      return ex.getInfo();						\
    } catch ( ... ) {							\
      std::cerr << "Unexpected exception " << std::endl;		\
      return CalciumTypes::CPATAL;					\
    }									\
    if ( IsSameType< _name , cplx >::value ) { *nRead=_nRead/2;		\
      DEBTRACE( "-------- CalciumInterface(lecture Inter Part) IsSameType cplx -------------" ) \
      DEBTRACE( "-------- CalciumInterface(lecture Inter Part) _nRead  : " << _nRead ) \
      DEBTRACE( "-------- CalciumInterface(lecture Inter Part) *nRead  : " << *nRead ) \
    } else *nRead = _nRead;						\
    if (dependencyType == CalciumTypes::CP_SEQUENTIEL ) \
        *ti=(CalTimeType< _type _qual >::TimeType)(_ti);			\
    DEBTRACE( "-------- CalciumInterface(lecture Inter Part), Data Ptr :" << *data ) \
    return CalciumTypes::CPOK;						\
  };									\
  									\
									\
  extern "C" void ecp_lecture_##_name##_free ( _type _qual * data) {	\
    CalciumInterface::ecp_free< _type, _name >(data);			\
  };		                                                        \
									\
									\
  extern "C" CalciumTypes::InfoType ecp_ecriture_##_name (void * component, int dependencyType, \
							  CalTimeType< _type _qual >::TimeType *t, \
							  long  i,	\
							  const char * const nomvar, size_t bufferLength, \
							  _type _qual * data ) { \
    Superv_Component_i * _component = static_cast<Superv_Component_i *>(component); \
    /* Je ne sais pas pourquoi, je n'arrive pas � passer t par valeur : corruption de la pile*/ \
    double         _t=*t;						\
    size_t         _bufferLength=bufferLength;				\
    if ( IsSameType< _name , cplx >::value ) _bufferLength=_bufferLength*2; \
    DEBTRACE( "-------- CalciumInterface(ecriture Inter Part) MARK 1 ------------------" ) \
    try {								\
      /*printf("-------- CalciumInterface(ecriture Inter Part), cp_name : Nom de la var. de type %s : %s\n",#_type,nomvar);*/ \
      DEBTRACE( "----------->-" << nomvar )				\
      CalciumInterface::ecp_ecriture< _type, _name >( *_component,	\
						      dependencyType,	\
						      _t,i,nomvar,_bufferLength,*data); \
    } catch ( const CalciumException & ex) {				\
      std::cerr << ex.what() << std::endl;				\
      return ex.getInfo();						\
    } catch ( ... ) {				\
      std::cerr << "Unexpected exception " << std::endl; \
      return CalciumTypes::CPATAL;						\
    }									\
    DEBTRACE( "-------- CalciumInterface(ecriture Inter Part), Valeur de data :" << data ) \
      return CalciumTypes::CPOK;					\
  };									\



#define STAR *
// Le premier argument est utilis�e :
//  - comme suffixe dans la d�finition des noms ecp_lecture_ , ecp_ecriture_ et ecp_free_
//  - comme second argument template � l'appel de la m�thode C++ correspondante
//      ( le type de port correspondant est alors obtenu par un trait)
// Le second argument est utilis�e :
// - pour typer le param�tre data de la proc�dure g�n�r�e 
// - pour d�duire le type des param�tres t, ti tf via un trait
// - comme premier param�tre template � l'appel de la m�thode C++ correspondante
//       (pour typer les donn�es pass�es en param�tre )
// Notons que dans le cas CALCIUM_C2CPP_INTERFACE_(int,int,), le type int n'existe pas
// en CORBA, le port CALCIUM correspondant utilise une s�quence de long. La m�thode
// C++ CALCIUM de lecture rep�re cette diff�rence de type et charge 
// le manipulateur de donn�es d'effectuer  une recopie (qui fonctionne si les types sont compatibles). 
CALCIUM_C2CPP_INTERFACE_(int,int,);
CALCIUM_C2CPP_INTERFACE_(float,float, );
CALCIUM_C2CPP_INTERFACE_(double,double,);
// Fonctionne mais essai suivant pour simplification de Calcium.c CALCIUM_C2CPP_INTERFACE_(bool,bool,);
CALCIUM_C2CPP_INTERFACE_(bool,int,);
CALCIUM_C2CPP_INTERFACE_(cplx,float,);
CALCIUM_C2CPP_INTERFACE_(str,char*,);

// INTERFACE C/CPP pour les chaines de caract�res
// Le param�tre suppl�mentaire strsize n'�tant pas utilis�
// j'utilise la g�n�ration par la macro CALCIUM_C2CPP_INTERFACE_(str,char*,);
// extern "C" CalciumTypes::InfoType ecp_lecture_str (void * component, int dependencyType, 
// 						   float * ti, float * tf, long * i, 
// 						   const char * const nomvar, size_t bufferLength, 
// 						   size_t * nRead, char ** *data, size_t strsize ) { 

//   Superv_Component_i * _component = static_cast<Superv_Component_i *>(component); 
//   double         _ti=*ti;						
//   double         _tf=*tf;						
//   size_t         _nRead=0;						
//   size_t         _bufferLength=bufferLength;				
//   CalciumTypes::DependencyType dependencyType=			
//     static_cast<CalciumTypes::DependencyType>(dependencyType);	
  
//   // - GERER POINTEUR NULL : NOTHING TODO 
//   // - VERIFIER LA TAILLE DES CHAINES RETOURNEES (ELLES DEVRAIENT ETRES CORRECTES SI L'ECRITURE EST BIEN CODEE.)

//   DEBTRACE( "-------- CalciumInterface(lecture Inter Part) MARK 1 ------------------" ) 
//     try {								
//       CalciumInterface::ecp_lecture< char*, char* >( *_component,	
// 						     dependencyType, 
// 						     _ti, _tf, *i,	
// 						     nomvar,		
// 						     _bufferLength, _nRead, *data); 
//     } catch ( const CalciumException & ex) {				
//       DEBTRACE( ex.what() );
//       return ex.getInfo();						
//     }									
    
//     *nRead = _nRead;						
    
//     if (dependencyType == CalciumTypes::CP_SEQUENTIEL ) 
//       *ti=(float)(_ti);			
    
//     DEBTRACE( "-------- CalciumInterface(lecture Inter Part), Data Ptr :" << *data ) ;

//     return CalciumTypes::CPOK;
//   };									
  									

// extern "C" void ecp_lecture_str_free (char** data) {	
//   CalciumInterface::ecp_free< char*, char* >(data);			
// };		                                                        
									
									
// extern "C" CalciumTypes::InfoType ecp_ecriture_str (void * component, int dependencyType, 
// 						    float *t, long  i,	
// 						    const char * const nomvar, size_t bufferLength, 
// 						    char ** data, int strsize ) { 

//     Superv_Component_i * _component = static_cast<Superv_Component_i *>(component); 
//     /* Je ne sais pas pourquoi, je n'arrive pas � passer t par valeur : corruption de la pile*/ 
//     double         _t=*t;						
//     size_t         _bufferLength=bufferLength;				

//     // - VERIFIER LA TAILLE DES CHAINES RETOURNEES (ELLES DEVRAIENT ETRES CORRECTES SI L'ECRITURE EST BIEN CODEE.)

//     DEBTRACE( "-------- CalciumInterface(ecriture Inter Part) MARK 1 ------------------" ) 
//     try {								
//       std::string essai(nomvar);					
//       DEBTRACE( "----------->-" << nomvar )		
// 	CalciumInterface::ecp_ecriture< char*, char* >( *_component,	
// 							static_cast<CalciumTypes::DependencyType>(dependencyType), 
// 							_t,i,nomvar,_bufferLength,*data); 
//     } catch ( const CalciumException & ex) {				
//       std::cerr << ex.what() << std::endl;				
//       return ex.getInfo();						
//     }									
//     DEBTRACE( "-------- CalciumInterface(ecriture Inter Part), Valeur de data :" << data ) 
//     return CalciumTypes::CPOK;						
//   };									


#endif
