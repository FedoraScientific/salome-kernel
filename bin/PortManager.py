#!/usr/bin/env python
#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2014  CEA/DEN, EDF R&D, OPEN CASCADE
#
# Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
# CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
# See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#
import os
import sys

try:
  import cPickle as pickle
except:
  import pickle

import logging
def createLogger():
  logger = logging.getLogger(__name__)
#  logger.setLevel(logging.DEBUG)
  ch = logging.StreamHandler()
  ch.setLevel(logging.DEBUG)
  formatter = logging.Formatter("%(levelname)s:%(threadName)s:%(message)s")
  ch.setFormatter(formatter)
  logger.addHandler(ch)
  return logger
#
logger = createLogger()

#------------------------------------
# A file locker (Linux only)
def __acquire_lock(lock):
  if sys.platform == "win32":
    import msvcrt
    # lock 1 byte: file is supposed to be zero-byte long
    msvcrt.locking(lock.fileno(), msvcrt.LK_LOCK, 1)
  else:
    import fcntl
    fcntl.flock(lock, fcntl.LOCK_EX)
#
def __release_lock(lock):
  if sys.platform == "win32":
    import msvcrt
    msvcrt.locking(lock.fileno(), msvcrt.LK_UNLCK, 1)
  else:
    import fcntl
    fcntl.flock(lock, fcntl.LOCK_UN)
#

def _getConfigurationFilename():
  omniorbUserPath = os.getenv("OMNIORB_USER_PATH")

  from salome_utils import generateFileName
  portmanager_config = generateFileName(omniorbUserPath,
                                        prefix="omniORB",
                                        suffix="PortManager",
                                        extension="cfg",
                                        hidden=True)
  lock_file = portmanager_config + "-lock"
  return (portmanager_config, lock_file)
#

def __isPortUsed(port, busy_ports):
  return (port in busy_ports) or __isNetworkConnectionActiveOnPort(port)
#

def __isNetworkConnectionActiveOnPort(port):
  # :NOTE: Under windows:
  #        netstat options -l and -t are unavailable
  #        grep command is unavailable
  from subprocess import Popen, PIPE
  (stdout, stderr) = Popen(['netstat','-an'], stdout=PIPE).communicate()
  import StringIO
  buf = StringIO.StringIO(stdout)
  ports = buf.readlines()
  # search for TCP - LISTEN connections
  import re
  regObj = re.compile( ".*tcp.*:([0-9]+).*:.*listen", re.IGNORECASE );
  for item in ports:
    try:
      p = int(regObj.match(item).group(1))
      if p == port: return True
    except:
      pass
#

def getPort(preferedPort=None):
  logger.debug("GET PORT")

  config_file, lock_file = _getConfigurationFilename()
  with open(lock_file, 'w') as lock:
    # acquire lock
    __acquire_lock(lock)

    # read config
    config = {'busy_ports':[]}
    logger.debug("read configuration file")
    try:
      with open(config_file, 'r') as f:
        config = pickle.load(f)
    except IOError: # empty file
      pass

    logger.debug("load busy_ports: %s"%str(config["busy_ports"]))

    # append port
    busy_ports = config["busy_ports"]
    port = preferedPort
    if not port or __isPortUsed(port, busy_ports):
      port = 2810
      while __isPortUsed(port, busy_ports):
        if port == 2810+100:
          msg  = "\n"
          msg += "Can't find a free port to launch omniNames\n"
          msg += "Try to kill the running servers and then launch SALOME again.\n"
          raise RuntimeError, msg
        port = port + 1
    logger.debug("found free port: %s"%str(port))
    config["busy_ports"].append(port)

    # write config
    logger.debug("write busy_ports: %s"%str(config["busy_ports"]))
    try:
      with open(config_file, 'w') as f:
        pickle.dump(config, f)
    except IOError:
      pass

    # release lock
    __release_lock(lock)

    logger.debug("get port: %s"%str(port))
    return port
#

def releasePort(port):
  port = int(port)
  logger.debug("RELEASE PORT (%s)"%port)

  config_file, lock_file = _getConfigurationFilename()
  with open(lock_file, 'w') as lock:
    # acquire lock
    __acquire_lock(lock)

    # read config
    config = {'busy_ports':[]}
    logger.debug("read configuration file")
    try:
      with open(config_file, 'r') as f:
        config = pickle.load(f)
    except IOError: # empty file
      pass

    logger.debug("load busy_ports: %s"%str(config["busy_ports"]))

    # remove port from list
    busy_ports = config["busy_ports"]

    if port in busy_ports:
      busy_ports.remove(port)
      config["busy_ports"] = busy_ports

    # write config
    logger.debug("write busy_ports: %s"%str(config["busy_ports"]))
    try:
      with open(config_file, 'w') as f:
        pickle.dump(config, f)
    except IOError:
      pass

    # release lock
    __release_lock(lock)

    logger.debug("released port port: %s"%str(port))
#

def getBusyPorts():
  config_file, lock_file = _getConfigurationFilename()
  with open(lock_file, 'w') as lock:
    # acquire lock
    __acquire_lock(lock)

    # read config
    config = {'busy_ports':[]}
    logger.debug("read configuration file")
    try:
      with open(config_file, 'r') as f:
        config = pickle.load(f)
    except IOError: # empty file
      pass

    logger.debug("load busy_ports: %s"%str(config["busy_ports"]))

    busy_ports = config["busy_ports"]
    # release lock
    __release_lock(lock)

    return busy_ports
#