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

//  File   : CouplingPolicy.hxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-02-28 15:26:32 +0100 (mer, 28 fév 2007) $
// Id          : $Id$
//
#ifndef _COUPLING_POLICY_HXX_
#define _COUPLING_POLICY_HXX_

#include "IteratorTraits.hxx"
#include "FindKeyPredicate.hxx"
#include <algorithm>
#include <functional>
#include <iterator>

// La politique de couplage doit d�finir
// 1) le type DataId qui identifie une instance de donn�es 
// 2) Un container de DataId appel� DataIdContainer et son it�rateur
// 3) Un constructeur de DataIdContainer qui  initialise le container � partir d'un DataId

//////////  Le DataId //////////////
//   COUPLING_POLICY::DataId 
//   est construit par  DataId(T1 time,T2 tag)
//
//   COUPLING_POLICY::DataIdContainer
//   qui d�fini  
//   les types    : DataIdContainer::iterator 
//   les m�thodes : begin(), end(), empty()
//   COUPLING_POLICY::DataIdContainer::iterator 
//   qui d�fini 
//   les m�thodes : != , == , ++() , ()++, *(), =

//   COUPLING_POLICY::DataTable
//    std::map< DataId, DataType>      DataTable;

//   D�finir void COUPLING_POLICY::DataIdContainer(const DataId &, CouplingPolicy & )
//   qui initialise le container � partir d'un DataId

//   Op�rateur d'affichage d'un dataId (les types doivent �tre affichables)

  
class CouplingPolicy {

public:

  // Renvoie isEqual si le dataId attendu est trouv� dans storedDataIds :
  //   - l'it�rateur wDataIt1 pointe alors sur ce dataId
  // Cette m�thode doit �tre red�fini dans le mode de couplage s'il veut g�rer
  // le cas d'un  dataId damand� non trouv� mais encadrable 
  // par deux autres dataIds. La m�thode renvoie alors isBounded== true et : 
  //   - l'it�rateur wDataIt1 doit  �tre tel que :
  //     wDataIt1->first < wdataId < (wDataIt1+1)->first
  template < typename Container >
  bool isDataIdConveniant(Container & storedDatas, 
                          const typename Container::key_type & expectedDataId,
                          bool & isEqual ,  bool & isBounded, 
                          typename Container::iterator & wDataIt1 ) const {
    typedef typename Container::key_type   key_type;
    typedef typename Container::value_type value_type;
    typedef typename Container::iterator iterator;
    isBounded = false;
    FindKeyPredicate<value_type> fkp(expectedDataId);
    wDataIt1  = std::find_if(storedDatas.begin(),storedDatas.end(),fkp);
    isEqual   = (wDataIt1 != storedDatas.end());
    std::cout << "-------- Generic isDataIdConvenient : isEqual : " << isEqual << " , isBounded " << isBounded << std::endl;
    return isEqual || isBounded;
  }

  
  // M�thode Vide d�clar�e ici pour d�finir son nom dans GenericPort
  // Elle profite � tous les modes de couplages n'impl�mentant pas
  // de comportement particulier dans la m�thode get pour 
  // le cas d'une demande de dataId inexistant mais encadr� par deux autres
  template <typename DataManipulator, class EnableIf = void >
  struct BoundedDataIdProcessor{
    BoundedDataIdProcessor(const CouplingPolicy & couplingPolicy) {};
    template < typename Iterator, typename DataId > 
    void inline apply(typename iterator_t<Iterator>::value_type & data,
                      const DataId & dataId,
                      const Iterator  & it1) const {
      typedef typename iterator_t<Iterator>::value_type value_type;
      std::cout << "-------- Generic BoundedDataIdProcessor.apply() called " << std::endl;

    }
  };

  // Supprime un DataId et ses donn�es associ�es
  // du container
  // Cette m�thode utilis�e dans GenericPort::Get 
  // peut �tre surcharg�e pour par exemple
  // conserver un historique.
  template <typename DataManipulator>
  struct EraseDataIdProcessor {

    EraseDataIdProcessor(CouplingPolicy couplingPolicy) {};

    template < typename Container >
    void apply(Container & storedDatas, 
               typename Container::iterator & wDataIt1 ) const {
      typedef typename Container::key_type   key_type;
      typedef typename Container::value_type value_type;
      typedef typename Container::iterator iterator;

      std::cout << "-------- Generic eraseDataId called " << std::endl;
    }
  };

  // Lorsque cette m�thode est appel�e l'expectedDataId n'a pas �t� trouv�
  // et n'est pas non plus encadr�e (en mode temporel)
  // Si l'on effectue pas de traitement particulier la m�thode renvoie false
  // Si le port a re�u une directive STOP une exception est lev�e
  // Si le port a re�u une directive CONTINUE, on localise l'expected
  template < typename DataManipulator > 
  struct DisconnectProcessor {

    DisconnectProcessor(const CouplingPolicy & couplingPolicy) {};

    template < typename Container, typename DataId >
    bool apply(Container & storedDatas,
               const DataId & expectedDataId,
               typename Container::iterator & wDataIt1 ) const {
      typedef typename Container::key_type   key_type;
      typedef typename Container::value_type value_type;
      typedef typename Container::iterator   iterator;

      std::cout << "-------- Generic DisconnectProcessor called " << std::endl;
      return true;
    }
  };

  // Remove all DataId from a container before a given time or tag
  template <typename DataManipulator>
  struct EraseDataIdBeforeOrAfterTagProcessor {

    EraseDataIdBeforeOrAfterTagProcessor(CouplingPolicy couplingPolicy) {};

    template < typename Container , typename TimeType , typename TagType >
    void apply(Container & storedDatas, TimeType time, TagType tag, bool before ) const {
      typedef typename Container::key_type   key_type;
      typedef typename Container::value_type value_type;
      typedef typename Container::iterator iterator;
    }
  };

  // Permet de r�veiller les m�thodes d'un GenericPort en attente
  // depuis une CouplingPolicy
  virtual void wakeupWaiting(){};

  virtual ~CouplingPolicy() {}

};

#endif
