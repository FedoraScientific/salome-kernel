using namespace std;
//=============================================================================
// File      : Utils_DESTRUCTEUR_GENERIQUE.cxx
// Created   : lun nov  5 16:04:47 CET 2001
// Author    : Antoine YESSAYAN, EDF
// Project   : SALOME
// Copyright : EDF 2001
// $Header$
//=============================================================================

using namespace std;

void Nettoyage( void ) ;

# include <iostream>
# include "utilities.h"
# include "Utils_DESTRUCTEUR_GENERIQUE.hxx"
# include <list>
extern "C"
{
# include <stdlib.h>
}

static list<DESTRUCTEUR_GENERIQUE_*> *Destructeurs=0 ;




/*! \class ATEXIT_
 *
 * M�canisme pour faire ex�cuter une seule fois DESTRUCTEUR_GENERIQUE_::Nettoyage
 * � la fin du traitement : creation d'un singleton statique de l'objet
 * tres specialise ATEXIT_.
 *
 * La cr�ation d'un objet de type ATEXIT_ entra�ne l'inscription de la fonction
 * Nettoyage() par atexit(). Il suffit donc de cr�er un singleton statique du type ATEXIT_
 * pour effectuer cet enregistrement une seule fois ind�pendament de l'utilisateur.
 */

class ATEXIT_
{
public :
	/*!
	 * Allocation dynamique de Destructeurs, une liste cha�n�e de DESTRUCTEUR_GENERIQUE_* et enregistrement
	 * de la fonction Nettoyage() par atexit().
	 *
	 * La liste cha�n�e Destructeurs est d�truite dans la fonction Nettoyage.
	 */
	ATEXIT_( void )
	{
		ASSERT (Destructeurs==0);
		Destructeurs = new list<DESTRUCTEUR_GENERIQUE_*> ; // Destructeurs allou� dynamiquement (cf. ci-dessous) ,
								   // il est utilis� puis d�truit par la fonction Nettoyage
		int cr = atexit( Nettoyage );                      // ex�cute Nettoyage lors de exit, apr�s la destruction des donn�es statiques !
		ASSERT(cr==0) ;
	}

	~ATEXIT_( )
	{
		MESSAGE("Destruction ATEXIT") ;
	}
};




static ATEXIT_ nettoyage ;	/* singleton statique */


/*!
 * traitement effectu� :
 * -# ex�cution de tous les objets de type DESTRUCTEUR_DE_ stock�s dans la liste Destructeurs (ce qui d�truit les
 *    singletons correspondant) ;
 * -# puis destruction de tous les objets de type DESTRUCTEUR_DE_ stock�s dans la liste Destructeurs;
 * -# destruction de la liste Destructeurs.
 */

void Nettoyage( void )
{
	BEGIN_OF("Nettoyage( void )") ;
	ASSERT(Destructeurs) ;
	SCRUTE( Destructeurs->size() ) ;
	if( Destructeurs->size() )
	{
		list<DESTRUCTEUR_GENERIQUE_*>::iterator it = Destructeurs->end() ;

		do
		{
			MESSAGE( "DESTRUCTION d'un SINGLETON") ;
			it-- ;
			DESTRUCTEUR_GENERIQUE_* ptr = *it ;
			//Destructeurs->remove( *it ) ;
			(*ptr)() ;
			delete ptr ;
		}while( it!=  Destructeurs->begin() ) ;

		Destructeurs->clear() ;
		SCRUTE( Destructeurs->size() ) ;
		ASSERT( Destructeurs->size()==0 ) ;
		ASSERT( Destructeurs->empty() ) ;
	}

	delete Destructeurs;
	Destructeurs=0;
	END_OF("Nettoyage( void )") ;
	return ;
}


/*!
 * Adds a destruction object to the list of actions to be performed at the end
 * of the process
 */
const int DESTRUCTEUR_GENERIQUE_::Ajout( DESTRUCTEUR_GENERIQUE_ &objet )
{
	// N.B. : l'ordre de creation des SINGLETON etant important
	//        on n'utilise pas deux fois la meme position pour
	//        les stocker dans la pile des objets.
	ASSERT(Destructeurs) ;
	Destructeurs->push_back( &objet ) ;
	return Destructeurs->size() ;
}
