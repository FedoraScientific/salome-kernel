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
#  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
#
#
#
#  File   : salome_shared_modules.py
#  Module : SALOME

from SALOME_utilities import *

"""
"""
MESSAGE( "Module salome_shared_modules" )

modules={}
try:
  # We try to import PyQt module. If present we import it
  # as a "shared" module
  import qt
  modules["qt"]=qt.__dict__
except:
  pass

# We keep in modules a copy of dictionnary modules
# that need to be imported only once in multi-study context

# Specific case : omniORB
import omniORB
modules["omniORB"]=omniORB.__dict__.copy()
import omniORB.CORBA
modules["omniORB.CORBA"]=omniORB.CORBA.__dict__.copy()
modules["CORBA"]=modules["omniORB.CORBA"]
import CosNaming
modules["CosNaming"]=CosNaming.__dict__.copy()
# end omniORB

#
# We search all Python CORBA (omniorb) modules.
# A Python CORBA module has 2 associated Python packages 
# These packages are named : <module_name> and <module_name>__POA
#

# 
# SALOMEDS must be imported first, at least before any CORBA module
# that references it.
# It seems that import order of related CORBA modules is important
# Perhaps, it's not sufficient so you should complete the list ???
#
import SALOMEDS
import Engines

import glob,os,sys

repertoire=os.path.dirname(__file__)
path=[repertoire,]

SALOME_ROOT_DIR = os.getenv("SALOME_ROOT_DIR")
if SALOME_ROOT_DIR != None:
	path.append(os.path.join(SALOME_ROOT_DIR,"lib","python"+sys.version[:3],"site-packages","salome"))

SALOME_SITE_DIR = os.getenv("SALOME_SITE_DIR")
if SALOME_SITE_DIR != None:
        SALOME_SITE_NAME = os.getenv("SALOME_SITE_NAME")
        if SALOME_SITE_NAME != None:
		path.append(os.path.join(SALOME_SITE_DIR,"lib","python"+sys.version[:3],"site-packages",SALOME_SITE_NAME))

#path=[repertoire,
#      os.path.join(repertoire,"..","lib","python"+sys.version[:3],"site-packages","salome"),
#      os.path.join(SALOME_ROOT_DIR,"lib","python"+sys.version[:3],"site-packages","salome"),
#      os.path.join(SALOME_SITE_DIR,"lib","python"+sys.version[:3],"site-packages","salome"),
#     ]

MESSAGE( str(path) )

for rep in path:
   # Add rep directory in the Python path to be able to import modules 
   sys.path[:0]=[rep]
   listdir=glob.glob(os.path.join(rep,"*__POA"))
   for elem in listdir:
      if os.path.isdir(elem):
         # Found a directory (Python package) named *__POA 
         module__POA=os.path.basename(elem)
         module=module__POA[:-5]
         MESSAGE( "Import CORBA module: " + module + ".\n Directory: " + os.path.abspath(elem)[:-5] )
         mod=__import__(module)
         # force the reload of CORBA module to resolve all the include relations between modules
         # specific of omniORBpy implementation (1.5)
         reload(mod)
         modules[module]=mod.__dict__.copy()
   # Now we import modules found in shared_modules directory
   r=os.path.join(rep,"shared_modules")
   MESSAGE( r )
   if os.path.isdir(r):
      sys.path[:0]=[r]
      listfich=glob.glob(os.path.join(r,"*.py"))
      MESSAGE( str(listfich) )
      for m in listfich:
         module=os.path.basename(m)[:-3]
         MESSAGE( "Import module: " + module + ".\n Location: " + os.path.abspath(m) )
         mod=__import__(module)
         modules[module]=mod.__dict__.copy()
      # Don't keep r directory in the path to not pollute it
      del sys.path[0]

   # Don't keep rep directory in the path to not pollute it
   del sys.path[0]

# End of CORBA modules import

def import_shared_modules(sysmodules):
   """
      This function "imports" shared modules contained in modules dictionnary
      in sysmodules.
      All these modules are only copied and not completely imported (not executed)
   """
   # EDF-CCAR: 
   # Problem with omniORB : omniORB creates a C Python module named  _omnipy
   # this module has sub-modules : omni_func, ...
   # _omnipy is quite a package but import with Python sub-interpreters does not seem to work 
   # To make it work we need to add those sub-modules in sysmodules
   import _omnipy
   sysmodules["_omnipy.omni_func"]=_omnipy.omni_func
   sysmodules["_omnipy.poa_func"]=_omnipy.poa_func
   sysmodules["_omnipy.poamanager_func"]=_omnipy.poa_func
   sysmodules["_omnipy.orb_func"]=_omnipy.orb_func

   import imp

   # All modules in the modules dictionnary are only copied, not completely imported 
   for nom_module,module_dict in modules.items():
      if sysmodules.has_key(nom_module):continue
      m=imp.new_module(nom_module)
      m.__dict__.update(module_dict)
      sysmodules[nom_module]=m

