//  Copyright (C) 2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//
//
//  File   : CalciumCxxInterface.hxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-03-01 13:27:58 +0100 (jeu, 01 mar 2007) $
// Id          : $Id$

#ifndef _CALCIUM_CXXINTERFACE_HXX_
#define _CALCIUM_CXXINTERFACE_HXX_

#include <string>
#include <vector>
#include <iostream>
#include "Superv_Component_i.hxx"
#include "CalciumException.hxx"
#include "CalciumTypes.hxx"
#include "CalciumGenericUsesPort.hxx"
#include "Copy2UserSpace.hxx"
#include "Copy2CorbaSpace.hxx"
#include "CalciumPortTraits.hxx"

#include <stdio.h>

//#define _DEBUG_

template <typename T1, typename T2>
struct IsSameType {
  static const bool value = false;
};
template <typename T1>
struct IsSameType<T1,T1> {
  static const bool value = true;
};



namespace CalciumInterface {
  
  /********************* INTERFACE DE DECONNEXION *****************/

  static void
  ecp_fin (Superv_Component_i & component, bool provideLastGivenValue)
  { 
    std::vector<std::string> usesPortNames;
    std::vector<std::string>::const_iterator it;
    component.get_uses_port_names(usesPortNames);    
    
    //r�cup�rer le type de r�el du port est un peu difficile
    //car l'interface nous donne aucune indication

    //     uses_port *myUsesPort;
    calcium_uses_port* myCalciumUsesPort;
      
    for (it=usesPortNames.begin(); it != usesPortNames.end(); ++it) {
      try {

	myCalciumUsesPort= 
	  component.Superv_Component_i::get_port< calcium_uses_port >((*it).c_str());

// 	component.Superv_Component_i::get_port(myUsesPort,(*it).c_str());
// 	calcium_uses_port* myCalciumUsesPort=
// 	  dynamic_cast<calcium_uses_port*>(myUsesPort);

#ifdef _DEBUG_
	std::cerr << "-------- CalciumInterface(ecp_fin) MARK 1 -|"<< *it <<"|----"<< 
	  //	  typeid(myUsesPort).name() <<"-------------" <<
	  typeid(myCalciumUsesPort).name() <<"-------------" << std::endl;
#endif
	
// 	if ( !myCalciumUsesPort )
// 	  throw Superv_Component_i::BadCast(LOC(OSS()<<"Impossible de convertir le port "
// 						<< *it << " en port de type calcium_uses_port." ));

	myCalciumUsesPort->disconnect(provideLastGivenValue);

      } catch ( const Superv_Component_i::BadCast & ex) {
#ifdef _DEBUG_
 	std::cerr << ex.what() << std::endl;
#endif
 	throw (CalciumException(CalciumTypes::CPTPVR,ex));
      } catch ( const DSC_Exception & ex) {
#ifdef _DEBUG_
	std::cerr << ex.what() << std::endl;
#endif
	// Exception venant de SupervComponent :
	//   PortNotDefined(CPNMVR), PortNotConnected(CPLIEN)  
	// ou du port uses : Dsc_Exception
	// On continue � traiter la deconnexion des autres ports uses
      } catch (...) {
 	throw (CalciumException(CalciumTypes::CPATAL,"Exception innatendue"));
	// En fonction du mode de gestion des erreurs throw;
      }
    }
  }


  /********************* INTERFACES DE DESALLOCATION  *****************/

  // Uniquement appel� par l'utilisateur s'il utilise la 0 copie
  //   ( pointeur de donn�es data==NULL � l'appel de ecp_lecture )
  // Une d�sallocation aura lieu uniquement si un buffer interm�diaire
  // �tait necessaire (type utilisateur et corba difff�rent)
  // La propri�t� du buffer est rendue � CORBA sinon  
  template <typename T1, typename T2> static void
  ecp_free ( T1 * dataPtr )
  {
    typedef typename ProvidesPortTraits<T2>::PortType     PortType;
    typedef typename PortType::DataManipulator            DataManipulator;
    typedef typename DataManipulator::Type                DataType; // Attention != T
    typedef typename DataManipulator::InnerType           InnerType;

    DeleteTraits<IsSameType<T1,InnerType>::value >::apply(dataPtr);
  }

  template <typename T1> static void
  ecp_free ( T1 * dataPtr )
  {
    ecp_free<T1,T1> ( dataPtr );
  }


  /********************* INTERFACES DE LECTURE *****************/

  // T1 est le type de donn�es
  template <typename T1 > static void
  ecp_lecture ( Superv_Component_i & component,
	       CalciumTypes::DependencyType dependencyType,
	       double        & ti,
	       double const  & tf,
	       long          & i,
	       const string  & nomVar, 
	       size_t          bufferLength,
	       size_t        & nRead, 
	       T1            * &data )
  {
    ecp_lecture<T1,T1> (component,dependencyType,ti,tf,
			i,nomVar,bufferLength,nRead,data);
  
  }

  // T1 est le type de donn�es
  // T2 est un <nom> de type Calcium permettant de s�lectionner le port correspondant 
  // T1 et T2 sont dissoci�s pour discriminer le cas des nombres complexes
  //  -> Les donn�es des nombres complexes sont de type float mais
  //     le port � utiliser est le port cplx
  template <typename T1, typename T2 > static void
  ecp_lecture ( Superv_Component_i & component,
	       CalciumTypes::DependencyType dependencyType,
	       double        & ti,
	       double const  & tf,
	       long          & i,
	       const string  & nomVar, 
	       size_t          bufferLength,
	       size_t        & nRead, 
	       T1            * &data )
  {

    assert(&component);

    typedef typename ProvidesPortTraits<T2>::PortType     PortType;
    typedef typename PortType::DataManipulator            DataManipulator;
    typedef typename DataManipulator::Type                CorbaDataType; // Attention != T1
    typedef typename DataManipulator::InnerType           InnerType;

    CorbaDataType     corbaData;

#ifdef _DEBUG_
    std::cerr << "-------- CalciumInterface(ecp_lecture) MARK 1 ------------------" << std::endl;
#endif

    if (nomVar.empty())
      throw CalciumException(CalciumTypes::CPNMVR,
				LOC("Le nom de la variable est <nul>"));
    PortType * port;
#ifdef _DEBUG_
    std::cout << "-------- CalciumInterface(ecp_lecture) MARK 2 ------------------" << std::endl;
#endif

    try {
      port  = component.Superv_Component_i::get_port< PortType > (nomVar.c_str());
#ifdef _DEBUG_
      std::cout << "-------- CalciumInterface(ecp_lecture) MARK 3 ------------------" << std::endl;
#endif
    } catch ( const Superv_Component_i::PortNotDefined & ex) {
#ifdef _DEBUG_
      std::cerr << ex.what() << std::endl;
#endif
      throw (CalciumException(CalciumTypes::CPNMVR,ex));
    } catch ( const Superv_Component_i::PortNotConnected & ex) {
#ifdef _DEBUG_
      std::cerr << ex.what() << std::endl;;
#endif
      throw (CalciumException(CalciumTypes::CPLIEN,ex)); 
      // VERIFIER LES CAS DES CODES : CPINARRET, CPSTOPSEQ, CPCTVR, CPLIEN
    } catch ( const Superv_Component_i::BadCast & ex) {
#ifdef _DEBUG_
      std::cerr << ex.what() << std::endl;
#endif
      throw (CalciumException(CalciumTypes::CPTPVR,ex));
    }
  
    // mode == mode du port 
    CalciumTypes::DependencyType portDependencyType = port->getDependencyType();

    if ( portDependencyType == CalciumTypes::UNDEFINED_DEPENDENCY )
      throw CalciumException(CalciumTypes::CPIT,
			     LOC(OSS()<<"Le mode de d�pendance de la variable " 
				 << nomVar << " est ind�fini."));

    if ( ( portDependencyType != dependencyType ) && 
	 ( dependencyType != CalciumTypes::SEQUENCE_DEPENDENCY ) ) 
      throw CalciumException(CalciumTypes::CPITVR,
			     LOC(OSS()<<"Le mode de d�pendance de la variable " 
				 << nomVar << ": " << portDependencyType 
				 << " ne correspond pas au mode demand�."));

  
    if ( dependencyType == CalciumTypes::TIME_DEPENDENCY ) {
      corbaData = port->get(ti,tf, 0);
#ifdef _DEBUG_
      std::cout << "-------- CalciumInterface(ecp_lecture) MARK 5 ------------------" << std::endl;
#endif
    } 
    else if ( dependencyType == CalciumTypes::ITERATION_DEPENDENCY ) {
      corbaData = port->get(0, i);
#ifdef _DEBUG_
      std::cout << "-------- CalciumInterface(ecp_lecture) MARK 6 ------------------" << std::endl;
#endif
    } else {
      // Lecture en s�quence
#ifdef _DEBUG_
      std::cout << "-------- CalciumInterface(ecp_lecture) MARK 7 ------------------" << std::endl;
#endif
      corbaData = port->next(ti,i);
    }
 
#ifdef _DEBUG_
    std::cout << "-------- CalciumInterface(ecp_lecture) MARK 8 ------------------" << std::endl;
#endif
    size_t corbaDataSize = DataManipulator::size(corbaData);
#ifdef _DEBUG_
    std::cout << "-------- CalciumInterface(ecp_lecture) corbaDataSize : " << corbaDataSize << std::endl;
#endif
   
    // V�rifie si l'utilisateur demande du 0 copie
    if ( data == NULL ) {
      if ( bufferLength != 0 ) {
	MESSAGE("bufferLength devrait valoir 0 pour l'utilisation du mode sans copie (data==NULL)");
      }
      nRead = corbaDataSize;
      // Si les types T et InnerType sont diff�rents, il faudra effectuer tout de m�me une recopie
      if (!IsSameType<T1,InnerType>::value) data = new T1[nRead];
#ifdef _DEBUG_
      std::cout << "-------- CalciumInterface(ecp_lecture) MARK 9 ------------------" << std::endl;
#endif
      // On essaye de faire du 0 copy si les types T1 et InnerType sont les m�mes
      Copy2UserSpace< IsSameType<T1,InnerType>::value >::apply(data,corbaData,nRead);
#ifdef _DEBUG_
      std::cout << "-------- CalciumInterface(ecp_lecture) MARK 10 ------------------" << std::endl;
#endif
      // Attention : Seul CalciumCouplingPolicy via eraseDataId doit d�cider de supprimer ou non
      // la donn�e corba associ�e � un DataId ! Ne pas effectuer la desallocation suivante :
      // DataManipulator::delete_data(corbaData);
      //  old : Dans les deux cas la structure CORBA n'est plus utile 
      //  old : Si !IsSameType<T1,InnerType>::value l'objet CORBA est d�truit avec son contenu
      //  old : Dans l'autre cas seul la coquille CORBA est d�truite 
      //  L'utilisateur devra appeler ecp_free (version modifi�e) qui d�terminera s'il est necessaire
      //   de d�sallouer un buffer interm�diaire ( types diff�rents) ou de rendre la propri�t�
   } else {
      nRead = std::min < size_t > (corbaDataSize,bufferLength);
#ifdef _DEBUG_
      std::cout << "-------- CalciumInterface(ecp_lecture) MARK 11 ------------------" << std::endl;
#endif
      Copy2UserSpace<false>::apply(data,corbaData,nRead);
#ifdef _DEBUG_
      std::cout << "-------- CalciumInterface(ecp_lecture) MARK 12 ------------------" << std::endl;
#endif
      // Attention : Seul CalciumCouplingPolicy via eraseDataId doit d�cider de supprimer ou non
      // la donn�e corba associ�e � un DataId ! Ne pas effectuer la desallocation suivante :
      // DataManipulator::delete_data(corbaData);
   }
#ifdef _DEBUG_
    std::cout << "-------- CalciumInterface(ecp_lecture), Valeur de data : " << std::endl;
    std::copy(data,data+nRead,std::ostream_iterator<T1>(std::cout," "));
    std::cout << "Ptr :" << data << std::endl;

    std::cout << "-------- CalciumInterface(ecp_lecture) MARK 13 ------------------" << std::endl;
#endif
 
  
    return;
  }


  /********************* INTERFACES D'ECRITURE *****************/

  template <typename T1> static void
  ecp_ecriture ( Superv_Component_i & component,
		 CalciumTypes::DependencyType dependencyType,
		 double const  & t,
		 long   const  & i,
		 const string  & nomVar, 
		 size_t bufferLength,
		 T1  & data ) {
    ecp_ecriture<T1,T1> (component,dependencyType,t,i,nomVar,bufferLength,data); 
  }

  template <typename T1, typename T2> static void
  ecp_ecriture ( Superv_Component_i & component,
		 CalciumTypes::DependencyType dependencyType,
		 double const  & t,
		 long   const  & i,
		 const string  & nomVar, 
		 size_t bufferLength,
		 T1  & data ) 
  {
    
    assert(&component);

    //typedef typename StarTrait<TT>::NonStarType           T;
    typedef typename UsesPortTraits<T2>::PortType          PortType;
    typedef typename ProvidesPortTraits<T2>::PortType      ProvidesPortType;
    typedef typename ProvidesPortType::DataManipulator     DataManipulator;
    // Verifier que l'on peut d�finir UsesPortType::DataManipulator
    //    typedef typename PortType::DataManipulator            DataManipulator;
    typedef typename DataManipulator::Type                CorbaDataType; // Attention != T1
    typedef typename DataManipulator::InnerType           InnerType;

#ifdef _DEBUG_
    std::cerr << "-------- CalciumInterface(ecriture) MARK 1 ------------------" << std::endl;
#endif
    if ( nomVar.empty() ) throw CalciumException(CalciumTypes::CPNMVR,
						    LOC("Le nom de la variable est <nul>"));
    PortType * port;
#ifdef _DEBUG_
    std::cout << "-------- CalciumInterface(ecriture) MARK 2 ------------------" << std::endl;
#endif

    try {
      port  = component.Superv_Component_i::get_port< PortType > (nomVar.c_str());
#ifdef _DEBUG_
      std::cout << "-------- CalciumInterface(ecriture) MARK 3 ------------------" << std::endl;
#endif
    } catch ( const Superv_Component_i::PortNotDefined & ex) {
#ifdef _DEBUG_
      std::cerr << ex.what() << std::endl;
#endif
      throw (CalciumException(CalciumTypes::CPNMVR,ex));
    } catch ( const Superv_Component_i::PortNotConnected & ex) {
#ifdef _DEBUG_
      std::cerr << ex.what() << std::endl;;
#endif
      throw (CalciumException(CalciumTypes::CPLIEN,ex)); 
      // VERIFIER LES CAS DES CODES : CPINARRET, CPSTOPSEQ, CPCTVR, CPLIEN
    } catch ( const Superv_Component_i::BadCast & ex) {
#ifdef _DEBUG_
      std::cerr << ex.what() << std::endl;
#endif
      throw (CalciumException(CalciumTypes::CPTPVR,ex));
    }
 
    // mode == mode du port 
    // On pourrait cr�er la m�thode CORBA dans le mode de Couplage CALCIUM.
    // et donc ajouter cette cette m�thode uniquement dans l'IDL calcium !

//     CalciumTypes::DependencyType portDependencyType;
//     try {
//       portDependencyType = port->getDependencyType();
//       std::cout << "-------- CalciumInterface(ecriture) MARK 4 ------------------" << std::endl;
//     } catch ( const DSC_Exception & ex ) {
//       std::cerr << ex.what() << std::endl;;
//       throw (CalciumException(CalciumTypes::CPIT,ex));
//     }

    if ( dependencyType == CalciumTypes::UNDEFINED_DEPENDENCY )
      throw CalciumException(CalciumTypes::CPIT,
				LOC(OSS()<<"Le mode de d�pendance demand� pour la variable " 
				    << nomVar << " est ind�fini."));

    if ( dependencyType == CalciumTypes::SEQUENCE_DEPENDENCY )
      throw CalciumException(CalciumTypes::CPIT,
				LOC(OSS()<<"Le mode de d�pendance SEQUENCE_DEPENDENCY pour la variable " 
				    << nomVar << " est impossible en �criture."));

    // Il faudrait que le port provides g�n�re une exception si le mode donn�e n'est pas
    // le bon. La seule fa�on de le faire est d'envoyer -1 en temps si on n'est en it�ration
    // et vice-versa pour informer les provides port du mode dans lequel on est. Sinon il faut
    // modifier l'interface IDL pour y ajouter un mode de d�pendance !
    // ---->
//     if ( portDependencyType != dependencyType ) 
//       throw CalciumException(CalciumTypes::CPITVR,
// 				LOC(OSS()<<"Le mode de d�pendance de la variable " 
// 				    << nomVar << " ne correspond pas au mode demand�."));

  
    if ( bufferLength < 1 )
      throw CalciumException(CalciumTypes::CPNTNULL,
				LOC(OSS()<<"Le buffer a envoyer est de taille nulle "));


#ifdef _DEBUG_
    std::cout << "-------- CalciumInterface(ecriture) MARK 4 ------------------" << std::endl;
#endif
    CorbaDataType corbaData;

    
    // Si les types Utilisateurs et CORBA sont diff�rents
    // il faut effectuer une recopie sinon on utilise directement le
    // buffer data pour constituer la s�quence
    // TODO : 
    // - Attention en mode asynchrone il faudra eventuellement
    //   faire une copie des donn�es m�me si elles sont de m�me type.
    // - OLD : En cas de collocalisation (du port provide et du port uses)
    //   OLD : il est necessaire d'effectuer une recopie du buffer car la
    //   OLD : s�quence est envoy�e au port provide par une r�f�rence sur 
    //   OLD : la s�quence locale. Or la m�thode put r�cup�re le buffer directement
    //   OLD : qui est alors le buffer utilisateur. Il pourrait alors arriver que :
    //   OLD :   * Le recepteur efface le buffer emetteur
    //   OLD :   * Le port lui-m�me efface le buffer de l'ulisateur !
    //   OLD : Cette copie est effectu�e dans GenericPortUses::put 
    //   OLD : en fonction de la collocalisation ou non.
    // - OLD :En cas de connection multiples d'un port uses distant vers plusieurs port provides
    //   OLD : collocalis�s les ports provides partagent la m�me copie de la donn�e ! 
    //   OLD : Il faut effectuer une copie dans le port provides.
    //   OLD : Cette copie est effectu�e dans GenericPortUses::put 
    //   OLD : en fonction de la collocalisation ou non.
    Copy2CorbaSpace<IsSameType<T1,InnerType>::value >::apply(corbaData,data,bufferLength);
 
    //TODO : GERER LES EXCEPTIONS ICI : ex le port n'est pas connect�
    if ( dependencyType == CalciumTypes::TIME_DEPENDENCY ) {
      try
      {
        port->put(*corbaData,t, -1); 
      }
      catch ( const DSC_Exception & ex) 
      {
        throw (CalciumException(CalciumTypes::CPATAL,ex.what()));
      }
      //Le -1 peut �tre trait� par le cst DataIdContainer et transform� en 0 
      //Etre oblig� de mettre une �toile ds (*corbadata) va poser des pb pour les types <> seq
#ifdef _DEBUG_
      std::cout << "-------- CalciumInterface(ecriture) MARK 5 ------------------" << std::endl;
#endif
    } 
    else if ( dependencyType == CalciumTypes::ITERATION_DEPENDENCY ) {
      try
      {
        port->put(*corbaData,-1, i);
      }
      catch ( const DSC_Exception & ex) 
      {
        throw (CalciumException(CalciumTypes::CPATAL,ex.what()));
      }
#ifdef _DEBUG_
      std::cout << "-------- CalciumInterface(ecriture) MARK 6 ------------------" << std::endl;
#endif
    } 

    
#ifdef _DEBUG_
    std::cout << "-------- CalciumInterface(ecriture), Valeur de corbaData : " << std::endl;
    for (int i = 0; i < corbaData->length(); ++i)
      cout << "-------- CalciumInterface(ecriture), corbaData[" << i << "] = " << (*corbaData)[i] << endl;
#endif
    
    //    if ( !IsSameType<T1,InnerType>::value ) delete corbaData;
    // Supprime l'objet CORBA avec eventuellement les donn�es qu'il contient (cas de la recopie)
    delete corbaData;

#ifdef _DEBUG_
    std::cout << "-------- CalciumInterface(ecriture) MARK 7 ------------------" << std::endl;
#endif
   
    return;
  }

};

#endif
