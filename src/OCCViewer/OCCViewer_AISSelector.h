//  SALOME OCCViewer : build OCC Viewer into Salome desktop
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : OCCViewer_AISSelector.h
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#ifndef OCCViewer_AISSELECTOR_H
#define OCCViewer_AISSELECTOR_H

#include "QAD.h"
#include "OCCViewer_Selector.h"

// Open CASCADE Includes
#include <Quantity_NameOfColor.hxx>
#include <AIS_InteractiveContext.hxx>

class QAD_EXPORT OCCViewer_AISSelector : public OCCViewer_Selector
{
  Q_OBJECT

  void	initialize();
  void	cleanup();

public:
  OCCViewer_AISSelector( const Handle (AIS_InteractiveContext)& );
  ~OCCViewer_AISSelector();

public:
  bool	moveTo ( int, int, const Handle (V3d_View)& );
  bool	select ( int, int, int, int, const Handle (V3d_View)& );
  bool	shiftSelect ( int, int, int, int, const Handle (V3d_View)& );
  bool	select ();
  bool	shiftSelect ();

  void	setContext ( const Handle (AIS_InteractiveContext)& );
  void	setHilightColor ( Quantity_NameOfColor color );
  void	setSelectColor ( Quantity_NameOfColor color );

protected:
  bool	checkSelection ( AIS_StatusOfPick status, bool hadSelection, bool addTo );

protected:
  Handle (AIS_InteractiveContext) myAISContext;	  // graphic context
  Quantity_NameOfColor		  myHilightColor; // color for hilight object
  Quantity_NameOfColor		  mySelectColor; // color for selected object

};

#endif

