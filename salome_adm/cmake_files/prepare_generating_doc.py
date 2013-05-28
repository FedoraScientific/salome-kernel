#!/usr/bin/env python
# Copyright (C) 2007-2012  CEA/DEN, EDF R&D, OPEN CASCADE
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License.
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

import sys, re
outfile = open(sys.argv[1], 'wb')
isCom = False
for line in open(sys.argv[2], 'rb').readlines():
    if re.match('class '+sys.argv[3]+'DC', line): 
        continue
    n = line.find('"""')
    n1 = line[(n+2):].find('"""')
    if (n > -1) and (n1 > -1):
        continue
    if isCom:
        if n > -1:
            isCom = False
        continue
    else:
        if n > -1:
            isCom = True
            continue       
    line = re.sub(r'^\s+#', '#', line) 
    line = re.sub(r'^\s+def', 'def', line) 
    line = re.sub(sys.argv[3]+'DC', sys.argv[3], line)
    outfile.write(line)
outfile.close()