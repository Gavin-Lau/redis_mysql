#!/bin/env python
#-*- encoding=utf8 -*-

import os
import shutil

# Windows
if (os.path.isfile("RdsOnMysql.sdf")):
	os.remove("RdsOnMysql.sdf")
if (os.path.isfile("RdsOnMysql.v12.suo")):
	os.remove("RdsOnMysql.v12.suo")

#bin
if (os.path.isfile("./bin_x64/RdsOnMysql.exe")):
	os.remove("./bin_x64/RdsOnMysql.exe")
if (os.path.isfile("./bin_x64/RdsOnMysql.ilk")):
	os.remove("./bin_x64/RdsOnMysql.ilk")
if (os.path.isfile("./bin_x64/RdsOnMysql.pdb")):
	os.remove("./bin_x64/RdsOnMysql.pdb")

if (os.path.exists("./src/x64")):
	shutil.rmtree("./src/x64",True)

#Linux
