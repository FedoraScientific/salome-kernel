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

//  File   : DataIdFilter.hxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
//
/*   Module Filtre
 *   -------------
 *
 *   Implemente les fonctions de filtrage et conversion d'un port de DATASTREAM
 *
 *   Rappel des fonctions du Filtrage:
 *   --------------------------------
 *
 *   Dans une communication de type DATASTREAM, le destinataire indique � l'avance la liste
 *   des instances qu'il veut recevoir, c'est � dire celles qui lui sont n�cessaires.
 *   Il indique pour cela la liste des 'times' et la liste des 'tags' qui
 *   caract�risent les instances d�sir�es.
 *   Ces deux listes sont ind�pendantes. Toute instance dont les param�tres 'time' et
 *   'tag' figurent dans la liste des 'times' et respectivement dans la liste des
 *   'tags' est d�sir�e par le destinataire.
 *   Par la suite, une telle instance sera accept�e par le port-DATASTREAM. Les autres
 *   seront rejet�es.
 *
 *   Le filtrage consiste � limiter les valeurs possibles du param�tre TIME ou TAG (un
 *   entier). La liste des valeurs possibles est d�crite sous la forme d'une liste de
 *   valeurs ou de s�quences arithm�tiques de valeurs.
 *   Exemple: 
 *     La liste 1; 3; 30:34; 40:50:2 autorise les valeurs 1 et 3 et toutes les valeurs
 *     comprises entre 30 et 34 inclus et toutes les valeurs de la s�quence 40 � 50
 *     inclus par pas de 2, c'est � dire 40, 42, ... 50.
 *   On appelle r�gle �l�mentaire de filtrage celle sp�cifiant un �l�ment de la liste
 *   des valeurs autoris�es: soit une seule valeur, soit une s�quence de valeurs. Une
 *   s�quence de valeurs est sp�cifi�e par sa valeur de d�part, sa valeur de fin et
 *   son pas. Le filtrage est donc d�fini par une suite de r�gles de filtrage.
 *   La fonction �l�mentaire de configuration du filtrage sert � sp�cifier une r�gle
 *   de filtrage.
 *
 *   Rappels des fonctions de conversion:
 *   -----------------------------------
 *
 *   La conversion est intimement li�e au filtrage car seules les valeurs passant le
 *   filtre sont converties. La conversion n'est pas obligatoire. Une valeur de TIME ou TAG
 *   entrante peut ne pas �tre convertie. Elle garde alors sa valeur et est gard�e
 *   telle quelle pour l'objet destinataire.
 *   DATASTREAM peut associer une r�gle de conversion � chaque r�gle �l�mentaire de
 *   filtrage.
 *   La conversion consiste � changer:
 *     - un valeur de TIME ou TAG en une valeur diff�rente
 *     - une s�quence de valeurs en une autre s�quence de valeurs de m�me taille
 *       (ex: 30:40 en 300:400:10)
 *   Mais la conversion permet aussi de transformer:
 *     - une valeur de TIME ou TAG unique en une s�quence de valeurs (les donn�es entrantes sont
 *       alors duppliqu�es et � chaque fois que l'objet destinataire r�clame une donn�e
 *       de la s�quence, il re�oit en fait une copie de la donn�e re�ue une seule fois)
 *
 *     - une s�quence de valeurs en une valeur unique (alors, chaque donn�e entrante
 *       associ�e � un TIME ou TAG de la s�quence correspond � une donn�e unique pour le
 *       destinataire: seule la derni�re re�ue est la donn�e valide)
 *
 */

#include <vector>
#include <iostream>

// Pour l'utilisation de "vector" de la STL
// Classe filtre_elementaire
//
// Impl�mente une structure de donn�e d�crivant un filtre �l�mentaire
// sur le param�tre TIME ou TAG; c'est
//    - soit une valeur enti�re unique
//    - soit une s�quence arithm�tique de valeurs
//
class filtre_elementaire
{
public:
    int len;    // Longueur de s�quence ou 1 pour une valeur unique
    int debut;  // D�but de la s�quence ou valeur pour une valeur unique
    int fin;    // Fin de la s�quence
    int pas;    // Pas de la s�quence

    // Constructeur par d�faut
    filtre_elementaire() {}
    
    // Cr�ation d'un filtre �l�mentaire pour une valeur unique
    filtre_elementaire(int valeur)
    {
        this->len = 1;
        this->debut = valeur;
    }

    // Cr�ation d'un filtre �l�mentaire pour une s�quence de valeurs enti�res
    // Le pas par d�faut est 1
    filtre_elementaire (int _debut, int _fin, int _pas=1)
    {
        this->debut = _debut;
        this->len = (_fin - _debut) / _pas;
        if (this->len > 0)
        {
            this->fin = _debut + _pas * this->len;  // Calcule la vrai borne de fin
            this->pas = _pas;
            this->len += 1;   // Compte les bornes et non les intervalles
        }
        else  // erreur de sp�cification: on ne prend que la premi�re valeur
            this->len = 1;
    }

    // Constructeur par copie
    filtre_elementaire (filtre_elementaire &_f)
    {
      this->len = _f.len;
      this->debut = _f.debut;
      this->fin = _f.fin;
      this->pas = _f.pas;
    }
};

// Classe filtre_conversion
//
// Impl�mente le filtrage et la conversion du param�tre TIME ou TAG
// des donn�es re�ues par un port DATASTREAM.
//
// Mode d'emploi:
//      1) Cr�ation d'un objet
//      2) Configuration de cet objet par passage de param�tres
//         de filtage et de conversion
//      3) A la cr�ation d'un port DATASTREAM, on passe au constructeur
//         deux objets 'filtre_conversion', l'un pour le TIME, l'autre pour le TAG.
//      4) A l'utilisation du port DATASTREAM, celui-ci appelle la m�thode
//         "applique_filtre_conversion" pour op�rer
//
class filtre_conversion
{
private:
    // Structure de donn�es d�crivant une conversion �l�mentaire:
    // un filtre �lementaire
    // et un pointeur �ventuel vers les param�tres de conversion associ�s
    class conversion_elementaire
    {
    public :
        // Data
        filtre_elementaire filtre;
        filtre_elementaire * p_convers;

        // Constructeur
        conversion_elementaire() {}

        // Constructeur par copie d'un objet non modifie (const)
        conversion_elementaire (const conversion_elementaire& _ce)
        {
            *this = _ce;
        }
        // Remarque: le Constructeur par copie d'un objet existe par defaut mais sans le modificateur 'const'
        //           et l'emploi d'un objet comme element dans un vecteur oblige d'avoir un tel const-copy-constructor.
    };

    // Donn�es de configuration de filtrage et conversion:
    //    une table de filtres �l�mentaires
    //    avec leurs donn�es de conversion associ�es �ventuelles
    std::vector<conversion_elementaire> config;

public:
    // Constructeur: juste une allocation m�moire initiale
    filtre_conversion() {}

    // Destructeur:
    // r�clamer la m�moire utilis�e par tous les �l�ments du vecteur config
    ~filtre_conversion()
    {
        std::vector<conversion_elementaire>::iterator i;
        for (i = this->config.begin(); i != this->config.end(); i ++)
        {
            delete (*i).p_convers;
        }
    }

    // Configuration partielle par ajout d'un filtre �l�mentaire
    bool config_elementaire (filtre_elementaire& _f)
    {
//    cout << "ajout config_elementaire 1  " << this << endl;
        conversion_elementaire conv_elem;
        
        conv_elem.filtre = _f;
        conv_elem.p_convers = NULL;

        // Ajoute cette conversion/filtrage elementaire a la liste
        this->config.push_back (conv_elem);
    
//    vector<conversion_elementaire>::iterator i;
//    cout << "liste apres ajout:" << endl;
//    for (i = this->config.begin(); i != this->config.end(); i ++)
//    {
//        cout << "config elem   " << endl;
//        cout << "filtre: len, debut, fin, pas " << (*i).filtre.len << " " << (*i).filtre.debut << " " << (*i).filtre.fin << " " << (*i).filtre.pas << endl;
//    }
        
        return true;
    }
    
    // Configuration partielle par ajout d'un filtre �l�mentaire
    // et sa conversion associ�e
    //
    // Retourne false si les param de conversion sont incompatibles avec le filtre �l�mentaire.
    // La configuration partielle est alors refus�e.
    //
    bool config_elementaire (filtre_elementaire& _f, filtre_elementaire& _conv)
    {
//    cout << "ajout config_elementaire 2  " << this << endl;
    
        if (_f.len == 1 || _conv.len == 1 || _f.len == _conv.len)
        {
            conversion_elementaire conv_elem;
            conv_elem.filtre = _f;
            conv_elem.p_convers = new filtre_elementaire(_conv);

            // Ajoute cette conversion/filtrage elementaire a la liste
            this->config.push_back (conv_elem);
    
//    vector<conversion_elementaire>::iterator i;
//    cout << "liste apres ajout:" << endl;
//    for (i = this->config.begin(); i != this->config.end(); i ++)
//    {
//        cout << "config elem   " << endl;
//        cout << "filtre: len, debut, fin, pas " << (*i).filtre.len << " " << (*i).filtre.debut << " " << (*i).filtre.fin << " " << (*i).filtre.pas << endl;
//    }
        
            return true;
        }
        else
        {
            // Filtre et conversion incompatibles
            return false;
        }
    }

    // applique_filtre_conversion: Op�ration du filtre et de la conversion
    template <typename T > T applique_filtre_conversion (T valeur_initiale, std::vector<T>& liste_conversions) const;
};



// filtre_conversion::applique_filtre_conversion: Op�ration du filtre et de la conversion
//
// Etant donn� une valeur enti�re (de TIME ou de TAG), cette m�thode d�termine :
//   - si cette valeur passe le filtre
//   - dans le cas o� une conversion existe, la liste des valeurs de conversion
//     qui correspondent � la valeur initiale
//
// Dans tous les cas, cette m�thode retourne une liste de valeurs.
// Dans le cas o� il n'y a pas de conversion, cette liste a une longueur 1
// et ne contient que la valeur initiale.
//
// Param�tre d'entr�e :         la valeur initiale (integer)
//
// Param�tre de sortie :        la liste des valeurs apr�s conversion (vector<int>)
//
// Valeur de retour :           la longueur de la liste
//     si cette longueur est 0, c'est que la valeur initiale ne passe pas le filtre
//
template <typename T>
T filtre_conversion::applique_filtre_conversion (T valeur_initiale, std::vector<T>& liste_conversions) const
{
    // Part d'une liste vierge
    liste_conversions.clear();

//    cout << "config applique_filtre_conversion " << this << endl;
    
    // Balaye tous les �l�ments de configuration
    // et cherche pour chacun d'eux si la valeur initiale est pr�sente parmi les valeurs filtr�es

    // Pour tous les �l�ments de configuration du filtrage/conversion
    std::vector<conversion_elementaire>::const_iterator i;
    for (i = config.begin(); i != config.end(); i ++)
    {

//    cout << "config elem   " << endl;
//    cout << "filtre: len, debut, fin, pas " << (*i).filtre.len << " " << (*i).filtre.debut << " " << (*i).filtre.fin << " " << (*i).filtre.pas << endl;
    
        bool si_passe_filtre = false;

        // Si la longueur du filtre est 1
        if ((*i).filtre.len == 1) {
          // Si la valeur initiale correspond � la valeur du filtre
          if ((*i).filtre.debut == valeur_initiale)
            si_passe_filtre = true;
        } else  {
          // Si la valeur initiale est dans la s�quence des valeurs du filtre
          //   la valeur est comprise dans les bornes [debut,fin]
          //   et sa distance du d�but de la s�quence est modulo le pas
          if (  ((*i).filtre.fin - valeur_initiale >= 0) == (valeur_initiale - (*i).filtre.debut >= 0)
                &&  (valeur_initiale - (*i).filtre.debut) % (*i).filtre.pas == 0  ) {
            si_passe_filtre = true;
          }
        }

        // Si la valeur initiale passe le filtre
        if (si_passe_filtre) {
          //    cout << "config: filtre passe    " << endl;
            
          // Si il y a une conversion � effectuer
          if ((*i).p_convers != NULL) {

            // Si la longueur du filtre est 1
            if ((*i).filtre.len == 1) {

              // Si la longueur des param�tres de conversion est aussi 1
              if ((*i).p_convers->len == 1) {
                // Ajoute la valeur de conversion � la liste des valeurs apr�s conversion
                liste_conversions.push_back ((*i).p_convers->debut);
              } else {
                // Ajoute la s�quence de conversion � la liste des valeurs apr�s conversion
                for (int s = (*i).p_convers->debut; s != (*i).p_convers->fin; s += (*i).p_convers->pas) {
                  liste_conversions.push_back (s);
                }
                liste_conversions.push_back ((*i).p_convers->fin);
              }

            } else {
              // Le filtre est une s�quence qui est convertie en une autre s�quence de m�me longueur
              // Choisit la valeur au rang d�sir� dans la s�quence de conversion
              int rang = (valeur_initiale - (*i).filtre.debut) / (*i).filtre.pas;

              int valeur_convertie = (*i).p_convers->debut + rang * (*i).p_convers->pas;

              // Ajoute cette valeur � la liste des valeurs apr�s conversion
              liste_conversions.push_back (valeur_convertie);
            }
          } else {
            // Ajoute la valeur initiale telle-quelle � la liste des valeurs apr�s conversion
            liste_conversions.push_back (valeur_initiale);
          }
        }
    }

    return liste_conversions.size();
}
