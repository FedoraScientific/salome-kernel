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

//  File   : CorbaTypeManipulator.hxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-02-07 18:26:44 +0100 (mer, 07 fév 2007) $
// Id          : $Id$
//
#ifndef _CORBA_TYPE_MANIPULATION_HXX_
#define _CORBA_TYPE_MANIPULATION_HXX_

#include <iostream>
#include <cstring>
#include <omniORB4/CORBA.h>

//#define MYDEBUG

// Classes manipulation
// -------------------
//
// Ces diff�rentes classes permettent d'unifier la manipulation des
// diff�rents types de donn�es dans un port datastream
// Les donn�es sont mani�es par valeur ou par pointeur 
// pour �viter les recopies de gros volume de donn�es 

// Les classes pr�sentes quatre m�thodes :
// - clone
// - get_data
// - delete_data
// - dump
// et
// trois types :
// - Type      : Le type CORBA de la donn�e manipul�e
// - InType    : Le mapping CORBA pour un param�tre IN du type manipul�
// - InnerType : Type interne des valeurs d'un type contenant 

// Cette classe permet de manipuler des types CORBA 
// any, struct, union et sequence (utiliser plut�t les seq_manipulator)
// Ces types sont manipul�s par pointeur.
// Les donn�es re�ues de CORBA sont syst�matiquement
// dupliqu�es pour �tre conserv�es.
// Quelque soit le type de donn�e, les donn�es sont consid�r�es 
// comme une donn�e unique (retour de size() == 1)
template <typename T >
class user_type_manipulation
{
public:
  typedef T *       Type;
  // correspond au mapping corba des type any, struct, 
  //                  union, s�quence en param�tre IN
  typedef const T & CorbaInType; 
  typedef T         InnerType;

  // Operation de recuperation des donnees venant de l'ORB et
  //  creation d'une copie (memoire sp�cialement allouee)
  static inline Type get_data(CorbaInType data) {
    return new T(data);
  }

  // Pb si ownerShip == True car appel par l'utilisateur de relPointer !
  static inline InnerType * const getPointer(Type data, bool ownerShip = false) {
    return data;
  }

  static inline void relPointer(InnerType * dataPtr) {
    delete dataPtr;
  }

  // Operation de clonage : par defaut, creation d'une copie en memoire allouee pour l'occasion
  static inline Type clone(Type data) { 
    return new T (* data);
  } 
  static inline Type clone(CorbaInType data) {
    return new T (data);
  }

  // Operation de cr�ation
  static inline Type create (size_t size=1) { 
    return new T();
  } 

  // Operation de destruction d'une donnee
  static inline void delete_data(Type data) {
    delete data;
  }
  
  // Renvoie la taille de la donn�e
  static inline size_t size(Type data) { 
    return 1;
  } 

  // Dump de l'objet pour deboguage: neant car on ne connait pas sa structure
  static inline void dump (CorbaInType data) {}
};


// G�re les types CORBA atomiques ('Int', 'Char', 'Float', ...)
// G�re les types enums
// G�re les r�f�rences d'objets CORBA
// Ces types sont manipul�s par valeur
// Les m�thodes getPointer ... ne devrait pas �tre utilis�e
// pour ce types de donn�es
template <typename T>
class atom_manipulation
{
public:
  typedef T Type;
  // correspond au mapping corba des types simples en param�tre IN
  typedef T CorbaInType; 
  typedef T InnerType; 

    
  // Operation de recuperation des donnees venant de l'ORB : une copie par affectation simple
  static inline Type get_data(CorbaInType data) {
    return data;
  }

 static inline InnerType * const getPointer(Type & data, bool getOwnerShip = false) {
//    InnerType * ptr;
//     if (getOwnerShip) {
//       ptr =new InnerType[1];*ptr=data;
//       return ptr;
//     } else
//      return &data;
   return &data;
 }

//   static inline void relPointer(InnerType * dataPtr) {
//     return;
//         delete[] dataPtr;
//   }

// Je ne sais pas comment l'impl�menter sans faire
// d'allocation heap
//static inline InnerType * allocPointer(size_t size=1) {
//    return  new InnerType[1];
  //}

  // Operation de clonage : une copie par affectation simple
  static inline Type clone(Type data) {
    return data;
  }

  // Inutile car Type == CorbaInType
  //   static inline Type clone(CorbaInType data) {
  //     return data;
  //   }

  // Operation de cr�ation
//   static inline Type create(size_t size=1,InnerType * data=NULL,
//                          bool giveOwnerShip=false) {
//     Type dummy;
//     if (dataPtr)
//       return *data;
//     else
//       return dummy;
//   } 
    
  // Operation de destruction d'une donnee: rien a faire car pas de memoire a liberer
  static inline void delete_data(Type data) {}
    // Renvoie la taille de la donn�e

  static inline size_t size(Type data) { 
    return 1;
  } 

  // Dump de l'objet pour deboguage : Affiche la donnee
  static void inline dump (CorbaInType data) {
    std::cerr << "[atom_manipulation] Data : " << data << std::endl;
  }
};


// G�re un type sequence de taille illimitee (Unbounded)
// Ces types sont manipul�s par pointeur
template <typename seq_T,typename elem_T>
class seq_u_manipulation {
  
public:
  typedef seq_T *       Type;        // Type de donn�e abstrait manipul� par GenericPort::Put,Get,..
  typedef const seq_T & CorbaInType; // Mapping corba de la s�quence en param�tre IN
  typedef elem_T        InnerType;   // Il n'existe pas dans CORBA de seq_T::elem_T
                                     // C'est la raison d'�tre du second param�tre template de seq_u_mani
 
  // Operation de recuperation des donnees venant de l'ORB
  // Remarque : On a un param�tre d'entr�e de type const seq_T &
  //            et en sortie un seq_T *
  static inline Type get_data(CorbaInType data) {
    CORBA::Long len = data.length();
    CORBA::Long max = data.maximum();
    // R�cup�re et devient propri�taire des donn�es re�ues dans la s�quence. 
    // La s�quence re�ue (mais pas le buffer) sera d�sallou�e au retour 
    // de la m�thode CORBA qui a re�u le type CorbaInType en param�tre
    // (ex: GenericPort::put)
    // REM : Le mapping CORBA du type s�quence IN est : const seq &

    // OLD : On ne teste pas si le flag release de la s�quence est � true ou false 
    // OLD : ( pour des s�quences de chaines ou d'objrefs )
    // OLD :   -> Si on est collocalis� le port uses doit cr�er une copie pour �viter la modification
    // OLD : du contenu de la s�quence lorsque l'utilisateur modifie ses donn�es dans son programme (0 copie)
    // OLD : ATTENTION TESTER p194 si le pointeur est null (release flag==false)
    // OLD :   -> La s�quence n'�tait pas propri�taire des donn�es !

    // Le flag release() de la s�quence est � false si elle n'est pas propri�taire du buffer
    // En  collocalit� release() renvoie false car 
    // l'appel� n'est pas propri�taire de la s�quence. On effectue alors
    // une copie pour �viter de perturber les structures de donn�es de l'appelant.
    // En non collocalis� on recr�e une s�quence avec le buffer de la premi�re dont on
    // a demand� la propri�t�.

#ifdef MYDEBUG
    std::cout << "----seq_u_manipulation::get_data(..)-- MARK 1 ------------------" << std::endl;
#endif
    if ( data.release() ) {
      InnerType * p_data = const_cast<seq_T &>(data).get_buffer(true);

    // Cr�e une nouvelle sequence propri�taire des donn�es du buffer (pas de recopie)
    // Les donn�es de la nouvelle s�quence seront automatiquement d�sallou�es 
    // par appel � la m�thode freebuf dans le destructeur de la s�quence (cf  delete_data).
#ifdef MYDEBUG
      std::cout << "----seq_u_manipulation::get_data(..)-- MARK 1(0 copy) bis ------"<<  p_data <<"------------" << std::endl;
#endif
    
      return  new seq_T (max, len, p_data, true);
    }
#ifdef MYDEBUG
    std::cout << "----seq_u_manipulation::get_data(..)-- MARK 1(recopie) bis ------"<<  &data <<"------------" << std::endl;
#endif
    // Cr�e une nouvelle sequence propri�taire des donn�es du buffer (avec recopie)    
    return new seq_T(data);

  }

  static inline size_t size(Type data) { 
    return data->length();
  } 

  // Operation de destruction d'une donnee
  static inline void delete_data(Type data) {
    //La s�quence est d�truite par appel � son destructeur
    //Ce destructeur prend en compte la n�cessit� de d�truire ou non
    //les donn�es contenues en fonction de son flag interne release()
    delete data;
  }

  // Operation de clonage : par defaut creation d'une copie en memoire allouee pour l'occasion
  // Utilisation du constructeur du type seq_T
  static inline Type clone(Type data) {
    return new seq_T (*data) ;
  }
  static inline Type clone(CorbaInType data) {
    return new seq_T (data);
  }

  // Permet d'obtenir un pointeur sur le buffer de la s�quence :
  // Si ownerShip=True, la s�quence n'est plus propri�taire du buffer
  //         (son pointeur de buffer interne est aussi r�initialis�) 
  //       On d�truit �galement explicitement la s�quence (mais pas le buffer !)
  // Si ownerShip=False, la s�quence reste propri�taire du buffer
  //    et l'utilisateur devra appeler delete_data sur la s�quence contenante pour
  //    d�truire � la fois la s�quence et le buffer contenu.
  static inline InnerType * const getPointer(Type data, bool ownerShip = false) {
    InnerType * p_data;
    if (ownerShip) {
      p_data = data->get_buffer(true);
      delete_data(data);
    } else
      p_data = data->get_buffer(false);
    return p_data;
  }

  // Permet de d�sallouer le buffer dont on d�tient le pointeur apr�s appel
  // � la m�thode getPointer avec ownerShip=True 
  static inline void relPointer(InnerType * dataPtr) {
    seq_T::freebuf(dataPtr);
  }

  // Permet d'allouer un buffer compatible avec le type s�quence
  static inline InnerType *  allocPointer(size_t size ) {
    return seq_T::allocbuf(size);
  }

  // Op�ration de cr�ation de la s�quence CORBA soit
  // - Vide et de taille size
  // - Utilisant les donn�es du pointeur *data de taille size 
  // (g�n�ralement pas de recopie qlq soit l'ownership )
  // data doit avoir �t� allou� par allocPointer si giveOwnerShip = true  
  static inline Type create(size_t size, InnerType * const data = NULL,
                            bool giveOwnerShip = false ) { 
    Type tmp;
    if (!data) {
      tmp = new seq_T();
      tmp->length(size);
    } else {
      tmp = new seq_T(size,size,data,giveOwnerShip); 
    }
    return tmp;
  } 

  // Copie le contenu de la s�quence dans le buffer idata de taille isize
  // pour les types non pointeur
  template <typename T >
  static inline void copy( Type data, T * const idata, size_t isize ) { 
    
    InnerType *dataPtr  = getPointer(data,false);

    for (int i = 0; i< isize; ++i) 
      idata[i]=dataPtr[i];

    // Le mode de recopie suivant ne permet pas  la conversion de type (ex int -> CORBA::Long)
    //OLD:     Type tmp = new seq_T(isize,isize,idata,false); 
    //OLD:     // giveOwnerShip == false -> seul le contenu du buffer data est d�truit et remplac�
    //OLD:     // par celui de data dans l'affectation suivante :
    //OLD:     //       ---> ATTENTION SI LA TAILLE DU BUFFER EST TROP PETITE, QUE FAIT CORBA !
    //OLD:     //              corruption m�moire
    //OLD:     // Cependant ce cas devrait pas arriv� (on s'assure dans les couches sup�rieures
    //OLD:     //  de la taille correcte du buffer de recopie)
    //OLD:     // Si giveOwnerShip �tait == true -> le buffer et son contenu serait d�truit puis une 
    //OLD:     // allocation de la taille du buffer de data serait effectu� avant la copie des donn�es  
    //OLD:     // tmp = data;
  } 

  // Copie le contenu de la s�quence de char* dans le buffer idata de taille isize
  // La g�n�ralisation de la recopie profonde est difficile du fait que CORBA ne renvoie pas
  // pas des objets de haut niveau de type std::vector<std::string> (avec des  interfaces d'acc�s identiques) 
  // mais un type simple C comme char *Tab[N]. On doit alors utiliser une m�thode de recopie sp�cifique
  // comme l'appel C strcpy.
  static inline void copy( Type data, char* * const idata, size_t isize ) { 

    char* *dataPtr  = getPointer(data,false);

    // Si idata[i] n'a pas �t� allou� suffisament grand,
    // il y a corruption de la m�moire
    for (int i = 0; i< isize; ++i) 
      strcpy(idata[i],dataPtr[i]);
  }
  
  // Dump de l'objet pour deboguage
  static void inline dump (CorbaInType data) {
    // Affiche la longueur des donnees
    std::cerr << "[seq_u_manipulation] Data length: " << data.length() << std::endl;
    // Affiche la longueur des donnees
    std::cerr << "[seq_u_manipulation] Data max: " << data.maximum() << std::endl;
  }
};


// TODO : V�rifier la conformit� de l'impl�mentation par rapport
//        au type unbounded

// G�re un type sequence de taille limit�e (bounded)
// Ces types sont manipul�s par pointeur
// Cette classe diff�re de la seq_u_manipulation
// par la signature du constructeur de la s�quence
// utilis� dans le methode get_data
template <typename seq_T,typename elem_T>
class seq_b_manipulation {
  
public:
  typedef seq_T *       Type;
  typedef const seq_T & CorbaInType;
  typedef elem_T        InnerType;


  // Operation de recuperation des donnees venant de l'ORB
  // Sans op�ration de notre part, ces donn�es seraient perdues
  // au retour de la m�thode put de GenericPort.
  // Remarque : On a un param�tre d'entr�e de type const seq_T &
  //            et en sortie un seq_T *
  static inline Type get_data(CorbaInType data) {
    CORBA::Long len = data.length();
    // R�cup�re et devient propri�taire des donn�es re�ues dans la s�quence 
    // la s�quence sera d�sallou� (mais pas le buffer)
    // au retour de la m�thode put (car mapping de type IN : const seq & )
     if ( data.release() ) {
       InnerType * p_data = const_cast<seq_T &>(data).get_buffer(true);

    // Cr�e une nouvelle sequence propri�taire des donn�es du buffer (g�n�ralement pas de recopie)
    // Les donn�es seront automatiquement d�sallou�es par appel interne � la m�thode freebuf
    // lors de la destruction de l'objet par appel � delete_data.
#ifdef MYDEBUG
    std::cout << "----seq_u_manipulation::get_data(..)-- MARK 1bis Pas de Duplication  -----------" << std::endl;
#endif
       return new seq_T (len, p_data, true);
     }
#ifdef MYDEBUG
    std::cout << "----seq_u_manipulation::get_data(..)-- MARK 1bis Duplication pour en devenir propri�taire -----------" << std::endl;
#endif
    // Cr�e une nouvelle sequence propri�taire des donn�es du buffer (avec recopie)    
    return new seq_T(data);

  }

  static inline size_t size(Type data) { 
    return data->length();
  } 

  // Operation de clonage : par defaut creation d'une copie en memoire allouee pour l'occasion
  // Utilisation du constructeur du type seq_T  
  static inline Type clone(Type data) {
    return new seq_T (* data);
  }
  static inline Type clone(CorbaInType data) {
    return new seq_T (data);
  }

  // Operation de destruction d'une donnee CORBA
  static inline void delete_data(Type data) {
    delete data;
  }

  // Permet d'obtenir un pointeur sur le buffer de la s�quence :
  // Si ownerShip=True, la s�quence n'est plus propri�taire du buffer
  //         (son pointeur de buffer interne est aussi r�initialis�) 
  //       On d�truit �galement explicitement la s�quence (mais pas le buffer !)
  // Si ownerShip=False, la s�quence reste propri�taire du buffer
  //    et l'utilisateur devra appeler delete_data sur la s�quence contenante pour
  //    d�truire � la fois la s�quence et le buffer contenu.
  static inline InnerType * const getPointer(Type data, bool getOwnerShip = false) {
    InnerType * p_data;
    if (getOwnerShip) {
      p_data = data->get_buffer(true);
      delete_data(data);
    } else
      p_data = data->get_buffer(false);
    return p_data;
  }

  // Permet de d�sallouer le buffer dont on d�tient le pointeur par appel
  // � la m�thode getPointer avec ownerShip=True si la s�quence contenante
  // � �t� d�truite.
  static inline void relPointer(InnerType * dataPtr) {
    seq_T::freebuf(dataPtr);
  }

  // Permet d'allouer un buffer pour la s�quence
  static inline InnerType *  allocPointer(size_t size ) {
    return seq_T::allocbuf(size);
  }

  // Operation de cr�ation du type corba soit
  // - Vide et de taille size
  // - Utilisant les donn�es du pointeur *data de taille size 
  // (g�n�ralement pas de recopie qlq soit l'ownership )
  // data doit avoir �t� allou� par allocPointer si giveOwnerShip = true  
  static inline Type create(size_t size, InnerType * const data = NULL,
                            bool giveOwnerShip = false ) { 
    Type tmp;
    if (!data) {
      tmp = new seq_T();
      tmp->length(size);
    } else {
      tmp = new seq_T(size,data,giveOwnerShip); 
    }
    return tmp;
  } 

  
  // Dump de l'objet pour deboguage
  static inline void dump (CorbaInType data) {
    // Affiche la longueur des donnees
    std::cerr << "[seq_b_manipulation] Data length: " << data.length() << std::endl;
  }
};

#endif
