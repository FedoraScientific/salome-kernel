// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
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

//  File   : Copy2UserSpace.hxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-02-13 11:09:09 +0100 (mar, 13 fév 2007) $
// Id          : $Id$
//
#ifndef _COPY_TO_USER_SPACE_HXX_
#define _COPY_TO_USER_SPACE_HXX_

#include <string>
#include <iostream>
#include "CalciumPortTraits.hxx"

#include <cstdio>

//#define MYDEBUG

//Les demandes de copies vers l'espace utilisateur
//proviennent d'une proc�dure de lecture  


//Cas du zero copie
template <bool zerocopy, typename DataManipulator >
struct Copy2UserSpace{
  
  template <class T1, class T2>
  static void apply( T1 * & data, T2 & corbaData, size_t nRead ){

     typedef typename DataManipulator::InnerType       InnerType;

    // OLD:Devient propri�taire des donn�es contenues dans la structure CORBA
    // OLD:(allou�es par allocbuff() pour une s�quence)
    // OLD:Le client est propri�taire des donn�es.
    // OLD:Il doit cependant �tre attentif au fait que s'il les modifie,
    // OLD:une nouvelle demande de lecture lui fournira les donn�es modifi�es.
    // OLD:TODO : Si plusieurs lecteurs demandent la m�me donn�e, 
    // OLD:       ? qui devient le propri�taire? --> Forc�ment le premier car
    // OLD:       ensuite la s�quence n'est plus propri�taire et rendra un pointeur NULL.
    // OLD:     NO: Le port devrait rest� propri�taire du contenu de la s�quence
    // OLD:     NO: L'utilisateur doit de toute les fa�ons utiliser les donn�es re�ues en
    // OLD:     NO: lecture seulement car si une nouvelle demande de lecture est formul�e
    // OLD:     NO: pour ces donn�es, les eventuelles modifications seraient visibles !
    // OLD:YES : La solution de donner la propri�t� � l'utilisateur est convenable car si
    // OLD:le port d�r�f�rence ces donn�es (garbage collecteur, niveau) le buffer
    // OLD:reste disponible � l'ulisateur en lecture et �criture
    // OLD:Le probl�me est que la donn�e CORBA stock�e par le port est maintenant vide (cf CORBA BOOK)
    // OLD:du coup quid d'une nouvelle demande de lecture : A TESTER 

     // Le PORT doit �tre capable de r�pondre aux demandes de lecture
     // multiples d'une donn�e pour une m�me estampille et doit donc garder un pointeur valide
     // sur le buffer. Il se pose cependant un probl�me s'il d�cide
     // de supprimer la donn�e alors que des client utilise le buffer (historique calcium) !
     // La seule fa�on de g�rer proprement cette situation est d'utiliser un shared_pointer (TODO).
     // Pour l'instant l'utilisateur du mode zero copie doit s'assurer que le niveau d'historique
     // utilis� par le port est compatible avec son utilisation des buffers. Il doit
     // �tre �galement conscient que s'il modifie le buffer, il est modifi� pour tous les
     // utilisateurs actuels et futurs.
    
     //REF:    InnerType * dataPtr  = DataManipulator::getPointer(corbaData,true);
     // Laisse la propri�t� des donn�es � la structure CORBA
     // (buffer allou�e par allocbuff() pour une s�quence)
     InnerType * dataPtr  = DataManipulator::getPointer(corbaData,false);

    // Cette ligne poserait uun probl�me dans la m�thode appelante, si elle
    // ne testait pas que les types utilisateurs et CORBA sont identiques :
    // ex :  InnerType == Corba::Long et d'un T == int
    // C'est l'objet de la sp�cialisation ci-dessous.
    data = dataPtr; 

    // En zero copie l'utilisateur doit appeler ecp_free ( cas ou un buffer interm�diaire
    // a �t� allou� pour cause de typage diff�rent xor necessit� de d�salouer le buffer allou� par CORBA)
    // L'utilisateur doit cependant �tre attentif au fait qu'apr�s d�sallocation, si la donn�e
    // est toujours estampill�e dans le port une nouvelle lecture pour cette estampille
    // rendrait un buffer vide.
  }
};

// Cas o� il faut effectuer une recopie
template <typename DataManipulator>
struct Copy2UserSpace<false, DataManipulator> {

  //Recopie le contenu de la donn�e CORBA dans le buffer utilisateur de longueur nRead
  template <class T1, class T2>
  static void apply( T1 * &data, T2 & corbaData, size_t nRead){

    typedef typename DataManipulator::InnerType        InnerType;
    
  
#ifdef MYDEBUG
    InnerType * dataPtr = NULL;
    // Affiche la valeur du pointeur de la structure corba
    //  et les pointeurs contenus le cas �ch�ant
    dataPtr  = DataManipulator::getPointer(corbaData,false);
    std::cerr << "-------- Copy2UserSpace<false> MARK 1a --dataPtr("<<dataPtr<<")[0.."<<
      DataManipulator::size(corbaData) <<"] : ----------------" << std::endl;
    std::copy(dataPtr,dataPtr+DataManipulator::size(corbaData),std::ostream_iterator<T1>(std::cerr," "));
    for (int i=0; i< DataManipulator::size(corbaData); ++i) 
      fprintf(stderr,"pointer[%d]=%p ",i, dataPtr[i]);
    std::cerr << std::endl;

    T1 * tmpData = data;
    //Cette affichage peut provoquer la d�tection d'�criture d'un espace non initailis�.
    std::cerr << "-------- Copy2UserSpace<false> MARK 1b --data("<<tmpData<<")[0.."<<
      DataManipulator::size(corbaData) <<"] : ----------------" << std::endl;
    std::copy(tmpData,tmpData+DataManipulator::size(corbaData),std::ostream_iterator<T1>(std::cerr," "));
    for (int i=0; i< DataManipulator::size(corbaData); ++i) 
      fprintf(stderr,"pointer[%d]=%p ",i, tmpData[i]);
    std::cerr << std::endl;
#endif

    // Pour les types pointeurs et ref il faut effectuer une recopie profonde.
    // On la d�l�gue au manipulateur de donn�es. 
      
    // Recopie des donn�es dans le buffer allou�e par l'utilisateur 
    // OU 
    // Recopie des donn�es dans le buffer allou�e par la m�thode appelante (ex: lecture)
    // dans le cas d'une demande utilisateur 0 copie mais que types utilisateurs et CORBA incompatibles.
    
    //std::copy(dataPtr,dataPtr+nRead,data);
    DataManipulator::copy(corbaData,data,nRead);
      
#ifdef MYDEBUG
    tmpData = data;
    std::cerr << "-------- Copy2UserSpace<false> MARK 1c --data("<<tmpData<<")[0.."<<
      DataManipulator::size(corbaData) <<"] : ----------------" << std::endl;
    std::copy(tmpData,tmpData+DataManipulator::size(corbaData),std::ostream_iterator<T1>(std::cerr," "));
    for (int i=0; i< DataManipulator::size(corbaData); ++i) 
      fprintf(stderr,"pointer[%d]=%p ",i, tmpData[i]);
    std::cerr << std::endl;
#endif
    
  }
  
};


// D�sallocation des buffers si necessaire
template <bool rel, typename DataManipulator >
struct DeleteTraits {
  template <typename T> 
  static void apply(T * dataPtr) {

    typedef typename DataManipulator::Type         DataType; // Attention != T
    
    // Attention : Seul CalciumCouplingPolicy via eraseDataId doit d�cider de supprimer ou non
    // la donn�e corba associ�e � un DataId ! 
    // Ne pas effectuer la desallocation suivante :
    // DataManipulator::relPointer(dataPtr);
  }
};

// D�salocation du buffer interm�diaire 
// dans le cas d'un type Utilisateur diff�rent du type CORBA 
template <typename DataManipulator>
struct DeleteTraits< false, DataManipulator > {

  template <typename T> 
  static void apply(T * dataPtr) { delete[] dataPtr; }

};

#endif
