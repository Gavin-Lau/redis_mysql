#!/usr/bin/python
# -*- coding: utf-8 -*-

##########################
# mysql odbc install autofile 
# python >= 2.7
##########################

import os
import sys
import platform
import subprocess

# check input dbtype 
dbtypes = []
curPath = os.path.abspath(os.curdir)
for i in os.listdir(curPath):
	if os.path.isdir(i):
		dbtypes.append(i)

if len(sys.argv) < 2:
	print "Useage:\n\t" + sys.argv[0] + " db-type\n"
	print "Available dbtypes:"
	print dbtypes
	sys.exit(-1)

curdbtype = sys.argv[1]
print "current db type = " + curdbtype

if ( curdbtype in dbtypes):
	print curdbtype + " db driver found!"
else:
	print curdbtype + " db is not surpported!"
	sys.exit(-1)
	
#build odbc lib full path	
systype = platform.system()
print "Current sys type is " + systype
driverPath = curPath + os.sep + curdbtype + os.sep + systype
cmdstr = '"' + driverPath + os.sep + 'bin' + os.sep + 'myodbc-installer"' + \
	' -d -a -n ' + '"MySQL5a"' + ' -t '

if systype == "Linux":
	cmdstr += '"DRIVER=/usr/local/lib/libmyodbc5a.so;' + \
		     'SETUP=/usr/local/lib/libmyodbc5S.so"'
elif systype == "Windows":
	cmdstr += '"DRIVER=' + driverPath + os.sep + 'myodbc5a.lib' + \
			'SETUP=' + driverPath + os.sep + 'myodbc5S.lib"'
print cmdstr

outstr = subprocess.check_output(cmdstr, shell=True)
print outstr
if outstr.find("Success") != -1:
	print "MySQL ODBC 5.3 ANSI Driver installed Success"
else:
	print "MySQL ODBC 5.3 ANSI Driver install fail"

