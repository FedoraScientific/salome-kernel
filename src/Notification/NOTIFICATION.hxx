//  SALOME Notification : wrapping of Notification service services
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
//  File   : NOTIFICATION.hxx
//  Author : Laurent DADA / Francis KLOSS
//  Module : SALOME

#ifndef NOTIFICATION_HXX
#define NOTIFICATION_HXX

#define NOTIF_WARNING "Warning"
#define NOTIF_STEP    "Step"
#define NOTIF_TRACE   "Trace"
#define NOTIF_VERBOSE "Verbose"

#define NOTIFICATION_ChannelName "EventChannel"

#include "utilities.h"

// kloss #include <omnithread.h>
#include "CosNotifyShorthands.h"

#include "NOTIFICATION_Supplier.hxx"
#include "NOTIFICATION_Consumer.hxx"

char* NOTIFICATION_date();
CosNA_EventChannel_ptr NOTIFICATION_channel();

#endif
