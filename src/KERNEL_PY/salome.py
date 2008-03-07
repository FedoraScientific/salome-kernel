#  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
#  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
# 
#  This library is free software; you can redistribute it and/or 
#  modify it under the terms of the GNU Lesser General Public 
#  License as published by the Free Software Foundation; either 
#  version 2.1 of the License. 
# 
#  This library is distributed in the hope that it will be useful, 
#  but WITHOUT ANY WARRANTY; without even the implied warranty of 
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
#  Lesser General Public License for more details. 
# 
#  You should have received a copy of the GNU Lesser General Public 
#  License along with this library; if not, write to the Free Software 
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
# 
# See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#
#
#
#  File   : salome.py
#  Author : Paul RASCLE, EDF
#  Module : SALOME
#  $Header$
"""
Module salome gives access to Salome ressources.

variables:

  salome.orb             : CORBA
  salome.naming_service  : instance of naming Service class
      methods:
          Resolve(name)  : find a CORBA object (ior) by its pathname
          Register(name) : register a CORBA object under a pathname
  salome.lcc             : instance of lifeCycleCORBA class
      methods:
          FindOrLoadComponent(server,name) :
                           obtain an Engine (CORBA object)
                           or launch the Engine if not found,
                           with a Server name and an Engine name
  salome.sg
      methods:
         updateObjBrowser(bool):
         getActiveStudyId():
         getActiveStudyName():

         SelectedCount():      returns number of selected objects
         getSelected(i):       returns entry of selected object number i
         getAllSelected():     returns list of entry of selected objects
         AddIObject(Entry):    select an existing Interactive object
         RemoveIObject(Entry): remove object from selection
         ClearIObjects():      clear selection

         Display(*Entry):
         DisplayOnly(Entry):
         Erase(Entry):
         DisplayAll():
         EraseAll():

         IDToObject(Entry):    returns CORBA reference from entry

  salome.myStudyName     : active Study Name
  salome.myStudyId       : active Study Id
  salome.myStudy         : the active Study itself (CORBA ior)
                           methods : defined in SALOMEDS.idl

"""

from salome_kernel import *
from salome_study import *
from salome_iapp import *

orb, lcc, naming_service, cm,sg=None,None,None,None,None
myStudyManager, myStudyId, myStudy, myStudyName=None,None,None,None

salome_initial=1
def salome_init(theStudyId=0,embedded=0):
    """
    Performs only once SALOME general purpose intialisation for scripts.
    optional argument : theStudyId
      When in embedded interpreter inside IAPP, theStudyId is not used
      When used without GUI (external interpreter)
        0      : create a new study (default).
        n (>0) : try connection to study with Id = n, or create a new one
                 if study not found.
                 If study creation, its Id may be different from theStudyId !
    Provides:
    orb             reference to CORBA
    lcc             a LifeCycleCorba instance
    naming_service  a naming service instance
    cm              reference to the container manager
    sg              access to SALOME GUI (when linked with IAPP GUI)
    myStudyManager  the study manager
    myStudyId       active study identifier
    myStudy         active study itself (CORBA reference)
    myStudyName     active study name
    """
    global salome_initial
    global orb, lcc, naming_service, cm
    global sg
    global myStudyManager, myStudyId, myStudy, myStudyName

    try:
        if salome_initial:
            salome_initial=0
            sg = salome_iapp_init(embedded)
            orb, lcc, naming_service, cm = salome_kernel_init()
            myStudyManager, myStudyId, myStudy, myStudyName =salome_study_init(theStudyId)
            pass
        pass
    except RuntimeError, inst:
        # wait a little to avoid trace mix
        import time
        time.sleep(0.2)
        x = inst
        print "salome.salome_init():", x
        print """
        ============================================
        May be there is no running SALOME session
        salome.salome_init() is intented to be used
        within an already running session
        ============================================
        """
        raise

#to expose all objects to pydoc
__all__=dir()
