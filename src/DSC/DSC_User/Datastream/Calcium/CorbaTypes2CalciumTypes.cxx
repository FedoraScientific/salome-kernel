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
// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-03-01 13:40:26 +0100 (Thu, 01 Mar 2007) $
// Id          : $Id$
//
#include "CorbaTypes2CalciumTypes.hxx"
#include <iostream>

using namespace std;

DATE_CAL_SCHEM::DATE_CAL_SCHEM() : map<Ports::Calcium_Ports::DateCalSchem,
				   CalciumTypes::DateCalSchem>()
{
  map<Ports::Calcium_Ports::DateCalSchem,
    CalciumTypes::DateCalSchem > &
    table  = ( map<Ports::Calcium_Ports::DateCalSchem,
	       CalciumTypes::DateCalSchem > & ) *this ;

table[Ports::Calcium_Ports::TI_SCHEM ] = CalciumTypes::TI_SCHEM ;
table[Ports::Calcium_Ports::TF_SCHEM ] = CalciumTypes::TF_SCHEM ;
table[Ports::Calcium_Ports::ALPHA_SCHEM ] = CalciumTypes::ALPHA_SCHEM ;
}


CalciumTypes::DateCalSchem DATE_CAL_SCHEM::operator[]( const Ports::Calcium_Ports::DateCalSchem &c ) const
{
  map<Ports::Calcium_Ports::DateCalSchem,
    CalciumTypes::DateCalSchem> &table = (map<Ports::Calcium_Ports::DateCalSchem,
					    CalciumTypes::DateCalSchem>&)*this ;
  assert( table.find( (Ports::Calcium_Ports::DateCalSchem)c ) != table.end() ) ;
  return table[ (Ports::Calcium_Ports::DateCalSchem)c ] ;
}

const DATE_CAL_SCHEM dateCalSchem ;



DEPENDENCY_TYPE::DEPENDENCY_TYPE() : map<Ports::Calcium_Ports::DependencyType,
				   CalciumTypes::DependencyType>()
{
  map<Ports::Calcium_Ports::DependencyType,
    CalciumTypes::DependencyType > &
    table  = ( map<Ports::Calcium_Ports::DependencyType,
	       CalciumTypes::DependencyType > & ) *this ;

table[Ports::Calcium_Ports::TIME_DEPENDENCY ]      = CalciumTypes::TIME_DEPENDENCY ;
table[Ports::Calcium_Ports::ITERATION_DEPENDENCY ] = CalciumTypes::ITERATION_DEPENDENCY ;
table[Ports::Calcium_Ports::UNDEFINED_DEPENDENCY ] = CalciumTypes::UNDEFINED_DEPENDENCY ;


#ifdef _DEBUG_
std::cerr << "DEPENDENCY_TYPE() : table["<<Ports::Calcium_Ports::TIME_DEPENDENCY<<"] : "<< 
  table[Ports::Calcium_Ports::TIME_DEPENDENCY] << std::endl;
std::cerr << "DEPENDENCY_TYPE() : table["<<Ports::Calcium_Ports::ITERATION_DEPENDENCY<<"] : "<< 
  table[Ports::Calcium_Ports::ITERATION_DEPENDENCY] << std::endl;
std::cerr << "DEPENDENCY_TYPE() : table["<<Ports::Calcium_Ports::UNDEFINED_DEPENDENCY<<"] : "<< 
  table[Ports::Calcium_Ports::UNDEFINED_DEPENDENCY] << std::endl;
#endif
}


CalciumTypes::DependencyType DEPENDENCY_TYPE::operator[]( const Ports::Calcium_Ports::DependencyType &c ) const
{
  map<Ports::Calcium_Ports::DependencyType,
    CalciumTypes::DependencyType> &table = (map<Ports::Calcium_Ports::DependencyType,
					    CalciumTypes::DependencyType>&)*this ;

#ifdef _DEBUG_
std::cerr << "DEPENDENCY_TYPE() : ::operator["<<c<<"]: " << table[c] << std::endl;
#endif

  assert( table.find( (Ports::Calcium_Ports::DependencyType)c ) != table.end() ) ;
  return table[ (Ports::Calcium_Ports::DependencyType)c ] ;
}

const DEPENDENCY_TYPE dependencyType ;




INTERPOLATION_SCHEM::INTERPOLATION_SCHEM() : map<Ports::Calcium_Ports::InterpolationSchem,
						 CalciumTypes::InterpolationSchem > () 
{
  map<Ports::Calcium_Ports::InterpolationSchem, 
    CalciumTypes::InterpolationSchem > &
    table  = ( map<Ports::Calcium_Ports::InterpolationSchem, 
	       CalciumTypes::InterpolationSchem > & ) *this ;

  table[Ports::Calcium_Ports::L0_SCHEM ] = CalciumTypes::L0_SCHEM ;
  table[Ports::Calcium_Ports::L1_SCHEM ] = CalciumTypes::L1_SCHEM ;
}


CalciumTypes::InterpolationSchem INTERPOLATION_SCHEM::operator[]( const Ports::Calcium_Ports::InterpolationSchem &c ) const
{
  map<Ports::Calcium_Ports::InterpolationSchem,
    CalciumTypes::InterpolationSchem> &table = 
    (map<Ports::Calcium_Ports::InterpolationSchem,
     CalciumTypes::InterpolationSchem>& ) *this ;

  assert( table.find( (Ports::Calcium_Ports::InterpolationSchem)c ) != table.end() ) ;
  return table[ (Ports::Calcium_Ports::InterpolationSchem)c ] ;
}

const INTERPOLATION_SCHEM interpolationSchem ;



EXTRAPOLATION_SCHEM::EXTRAPOLATION_SCHEM() : map<Ports::Calcium_Ports::ExtrapolationSchem,
						 CalciumTypes::ExtrapolationSchem > () 
{
  map<Ports::Calcium_Ports::ExtrapolationSchem, 
    CalciumTypes::ExtrapolationSchem > &
    table  = ( map<Ports::Calcium_Ports::ExtrapolationSchem, 
	       CalciumTypes::ExtrapolationSchem > & ) *this ;

  table[Ports::Calcium_Ports::E0_SCHEM ] = CalciumTypes::E0_SCHEM ;
  table[Ports::Calcium_Ports::E1_SCHEM ] = CalciumTypes::E1_SCHEM ;
  table[Ports::Calcium_Ports::UNDEFINED_EXTRA_SCHEM ] = CalciumTypes::UNDEFINED_EXTRA_SCHEM ;
}


CalciumTypes::ExtrapolationSchem EXTRAPOLATION_SCHEM::operator[]( const Ports::Calcium_Ports::ExtrapolationSchem &c ) const
{
  map<Ports::Calcium_Ports::ExtrapolationSchem,
    CalciumTypes::ExtrapolationSchem> &table = 
    (map<Ports::Calcium_Ports::ExtrapolationSchem,
     CalciumTypes::ExtrapolationSchem>& ) *this ;

  assert( table.find( (Ports::Calcium_Ports::ExtrapolationSchem)c ) != table.end() ) ;
  return table[ (Ports::Calcium_Ports::ExtrapolationSchem)c ] ;
}

const EXTRAPOLATION_SCHEM extrapolationSchem ;
