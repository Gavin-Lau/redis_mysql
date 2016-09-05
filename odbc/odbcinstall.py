#!/usr/bin/python
# -*- coding: utf-8 -*-

##########################
# mysql odbc install autofile 
##########################

import os
import sys
import platform

import subprocess
try: subprocess.check_output
except: subprocess.check_output = check_output

if len(sys.argv) < 2:
	print "Useage:\n\t" + sys.argv[0] + " db-type"
	sys.exit(-1)

curdbtype = sys.argv[1]
print "current db type = " + curdbtype

dbtypes = []
curPath = os.path.abspath(os.curdir)

for i in os.listdir(curPath):
	if os.path.isdir(i):
		dbtypes.append(i)
print dbtypes

if curdbtype in dbtypes:
	print curdbtype + " db driver found!"
else:
	print curdbtype + " db is not surpported!"
	
systype = platform.system()
libending = ""
libbegining = ""
print "Current sys type is " + systype
if systype == "Windows":
	libbegining = ""
	libending = "dll"
elif systype == "Linux":
	libbegining = "lib"
	libending = "so"

driverPath = curPath + os.sep + curdbtype + os.sep + systype
print driverPath

cmdstr = '"' + driverPath + os.sep + 'bin' + os.sep + 'myodbc-installer"' + \
		' -d -a -n ' + '"MySQL ODBC 5.3 ANSI Driver"' + ' -t ' + \
		'"DRIVER=' + driverPath + os.sep + 'lib' + os.sep + libbegining + 'myodbc5a.' + libending + ';' + \
		'SETUP=' + driverPath + os.sep + 'lib' + os.sep + libbegining + 'myodbc5S.' + libending + '"'
print cmdstr

outstr = subprocess.check_output(cmdstr, shell=True)
print outstr
if outstr.find("Success") != -1:
	print "MySQL ODBC 5.3 ANSI Driver installed Success"
else:
	print "MySQL ODBC 5.3 ANSI Driver install fail"
